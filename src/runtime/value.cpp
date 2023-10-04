class Value;
typedef shared_ptr<Value> Reference;

typedef bool Boolean;
typedef double Number;
typedef tuple<Number, Number> Range;
typedef string String;
typedef vector<Reference> Array;
typedef unordered_map<string, Reference> Table;

typedef shared_ptr<Table> Scope;

class Instruction;
class Block {
	public:
		Scope context;

		vector<string> arguments;
		vector<Instruction> defaults;
		vector<Instruction> body;

		function<Reference(Array)> binding = nullptr;

		static Reference Bound(function<Reference(Array)> binding);

		Block(){}
		Block(function<Reference(Array)> binding){ this->binding = binding; }
		Block(vector<string> arguments, vector<Instruction> defaults, vector<Instruction> body) {
			this->arguments = arguments;
			this->defaults = defaults;
			this->body = body;
		}
};

class Value {
	public:
		template <class T> static Reference Make(T one) { return Reference(new Value(one)); }
		template <class T1, class T2> static Reference Make(T1 one, T2 two) { return Reference(new Value(one, two)); }

		static Reference Lock(Reference value) { value->immutable = true; return value; }
		static Reference Copy(Reference from) { Reference copy = Empty(); copy->SET(from); return copy; }

		static Reference Empty() { return Reference(new Value()); }
		static Reference Empty(Type type) {
			if (type == Type::Array) {
				return Make(Array{});
			} else if (type == Type::Table) {
				return Make(Table{});
			}

			return Empty();
		}

		static Reference Locked() { return Lock(Empty()); }
		static Reference Locked(Type type) { return Lock(Empty(type)); }
		template <class T> static Reference Locked(T one) { return Make(one); }
		template <class T1, class T2> static Reference Locked(T1 one, T2 two) { return Make(one, two); }

		Value(){}

		Value(Boolean boolean) { type = Type::Boolean; value = boolean; }
		Value(int number) { type = Type::Number; value = Number(number); }
		Value(float number) { type = Type::Number; value = Number(number); }
		Value(Number number) { type = Type::Number; value = number; }
		Value(Number from, Number to) { type = Type::Range; value = make_tuple(from, to); }
		Value(String string) { type = Type::String; value = string; }
		Value(Array array) { type = Type::Array; value = array; }
		Value(Table table) { type = Type::Table; value = table; }
		Value(shared_ptr<Block> block) { type = Type::Block; this->block = block; }

		Value(Node* node) {
			if (node->mark == Mark::Word) {
				if (node->contents.compare("true") == 0) {
					type = Type::Boolean;
					value = true;
				} else if (node->contents.compare("false") == 0) {
					type = Type::Boolean;
					value = false;
				}
			} else if (node->mark == Mark::Number) {
				type = Type::Number;
				value = stod(node->contents);
			} else if (node->mark == Mark::String) {
				type = Type::String;
				value = node->contents;
			} else if (node->mark == Mark::Table) {
				type = Type::Table;
				Table table = Table{}; for (int i = 0; i < node->children.size(); i += 2) table[node->children[i].contents] = Value::Make(&node->children[i + 1]);
				value = table;
			} else if (node->mark == Mark::Array) {
				type = Type::Array;
				Array array = Array{}; for (int i = 0; i < node->children.size(); i++) array.push_back(Value::Make(&node->children[i]));
				value = array;
			} else if (node->mark == Mark::Block) {
				type = Type::Block;
				block = shared_ptr<Block>(new Block());
			}
		}

		bool is(Type type) { return this->type == type; }
		bool is(vector<Type> types) { for (auto &type : types) if (is(type)) return true; return false; }
		template <class T> T as() { return any_cast<T>(value); }

		Reference get(int index) { return as<Array>()[index]; }
		Reference get(string key) { return as<Table>()[key]; }

		Reference get(Reference index) {
			switch (type) {
				case Type::Array: return as<Array&>()[index->as<Number>()];
				case Type::Table: return as<Table&>()[index->as<String>()];
				default: return Empty();
			}
		}

		void set(Reference to, bool force=false) {
			if (!force && immutable) return;
			type = to->type;
			value = to->value;
			block = to->block;
		}

		void set(int index, Reference to, bool force=false) {
			if (!force && immutable) return;
			if (type != Type::Array) return;
			Array array = as<Array&>();
			if (index < 0 || index > array.size()) return;
			if (index == array.size()) array.push_back(Empty());
			array[index]->set(to);
			value = array;
		}

		void set(string key, Reference to, bool force=false) {
			if (!force && immutable) return;
			if (type != Type::Table) return;
			Table table = as<Table&>();
			if (!table.count(key)) table[key] = Empty();
			table[key]->set(to);
			value = table;
		}

		void SET(Reference to) { set(to, true); }
		void SET(int index, Reference to) { set(index, to, true); }
		void SET(string key, Reference to) { set(key, to, true); }

		bool isTrue() {
			if (type == Type::Void) return false;
			if (type == Type::Boolean) return as<Boolean>();
			if (type == Type::Number) return as<Number>() > 0;
			return true;
		}

		bool isFalse() { return !isTrue(); }

		bool equals(Reference other) {
			if (other->type != type) {
				if (type == Type::Boolean && other->type == Type::Void) return !as<Boolean>();
				if (type == Type::Void && other->type == Type::Boolean) return !other->as<Boolean>();
				if (type == Type::Number && other->type == Type::Void) return as<Number>() == 0;
				if (type == Type::Void && other->type == Type::Number) return other->as<Number>() == 0;
				if (type == Type::Boolean && other->type == Type::Number) return as<Boolean>() == other->as<Number>() > 0;
				if (type == Type::Number && other->type == Type::Boolean) return other->as<Boolean>() == as<Number>() > 0;
				
				return false;
			} else {
				if (type == Type::Void) return true;
				if (type == Type::Boolean) return as<Boolean>() == other->as<Boolean>();
				if (type == Type::Number) return as<Number>() == other->as<Number>();
				if (type == Type::Range) return as<Range>() == other->as<Range>();
				if (type == Type::String) return as<String>().compare(other->as<String>()) == 0;
			}

			return false;
		}

		string describe(Number number) {
			stringstream stream;
	    stream.precision(numeric_limits<Number>::digits10);
	    stream << number;

	    string representation = stream.str(); int cutoff = -1;
	    for (int i = 0; i < representation.size(); i++) if (cutoff < 0) {
	    	if (representation[i] == '.') cutoff = i + 1;
	    } else {
    		if (representation[i] != '0') cutoff = i + 1;
	    }

			return cutoff > -1
				? representation.substr(0, cutoff)
				: representation;
		}

		string describe(int indent = -1, string prefix="") {
			bool readable = indent > -1;
			string before = ( readable ? Utils::Indent(indent) : "" ) + prefix;

			if (is(Type::Boolean)) {
				return before + ( as<Boolean>() ? "true" : "false" );
			} else if (is(Type::Number)) {
				return before + describe(as<Number>());
			} else if (is(Type::Range)) {
				Range value = as<Range>();
				return before + "( " + describe(value.get<0>()) + " -> " + describe(value.get<1>()) + " )";
			} else if (is(Type::String)) {
				return before + "'" + as<String>() + "'";
			} else if (is(Type::Array)) {
				string output = readable ? "[\n" : "[ ";
				
				Array values = as<Array&>();
				for (int i = 0; i < values.size(); i++) {
					if (i > 0) output += readable ? ",\n" : ", ";
					output += values[i]->describe(readable ? indent + 1 : -1);
				}

				return before + output + ( readable ? "\n" + Utils::Indent(indent) + "]" : " ]" );
			} else if (is(Type::Table)) {
				string output = readable ? "{\n" : "{ ";
				
				Table values = as<Table&>();
				int i = 0; for (const auto& [ name, variable ] : values) {
					if (i > 0) output += readable ? ",\n" : ", "; i++;
					output += variable->describe(readable ? indent + 1 : -1, name + ": ");
				}

				return before + output + ( readable ? "\n" + Utils::Indent(indent) + "}" : " }" );
			} else if (is(Type::Block)) {
				return before + "'{ Block }'";
			}

			return before + "void";
		}

		bool immutable = false;
		Type type = Type::Void;
		shared_ptr<Block> block;
		any value;
};

Reference Block::Bound(function<Reference(Array)> binding) { return Value::Locked(shared_ptr<Block>(new Block(binding))); }