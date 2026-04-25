class Value;
typedef shared_ptr<Value> Reference;

typedef bool Boolean;
typedef double Number;
typedef tuple<Number, Number> Range;
typedef string String;
typedef vector<Reference> Array;
typedef tsl::ordered_map<string, Reference> Table;

typedef shared_ptr<Table> Scope;

struct Instruction;
class Block {
	public:
		vector<string> arguments;
		vector<vector<Instruction>> defaults;
		vector<Instruction> body;

		function<Reference(Array)> binding = nullptr;

		static Reference Bound(function<Reference(Array)> binding);

		Block(){}
		Block(function<Reference(Array)> binding) { this->binding = binding; }
		Block(vector<string> arguments, vector<vector<Instruction>> defaults, vector<Instruction> body){
			this->arguments = arguments;
			this->defaults = defaults;
			this->body = body;
		}
};

class Value : public std::enable_shared_from_this<Value> {
	public:
		template <class T> static Reference Make(T one) { return Reference(new Value(one)); }
		template <class T1, class T2> static Reference Make(T1 one, T2 two) { return Reference(new Value(one, two)); }

		static Reference Scan(Reference value, Reference name, function<Reference(Reference, Reference)> callback);
		static Array Scan(Array subject, function<Reference(Reference, Reference)> callback);
		static Table Scan(Table subject, function<Reference(Reference, Reference)> callback);

		static Reference Lock(Reference value) { value->immutable = true; return value; }
		static Reference Copy(Reference from) {
			Reference copy = Empty();
			copy->SET(from);
			return copy;
		}

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
		Value(const char* string) { type = Type::String; value = String(string); }
		Value(String string) { type = Type::String; value = string; }
		Value(Array array) { type = Type::Array; value = array; }
		Value(Table table) { type = Type::Table; value = table; }
		Value(shared_ptr<Block> block) { type = Type::Block; this->block = block; }

		Value(vector<String> source) {
			type = Type::Array;

			Array output;
			for (int i = 0; i < source.size(); i++) output.push_back(Value::Make(source[i]));

			value = output;
		}

		Value(Node* node);

		bool is(Type type) { return this->type == type; }
		bool is(vector<Type> types) { for (auto &type : types) if (is(type)) return true; return false; }
		template <class T> T as() { return *unsafe_any_cast<T>(&value); }
		template <class T> T* point() { return unsafe_any_cast<T>(&value); }

		Reference self() {
			Reference copy = Empty();
			copy->type = type;
			copy->value = value;
			copy->block = block;
			copy->context = context;
			return copy;
		}

		Reference get(int index) { return as<Array>()[index]; }
		Reference get(string key) { return as<Table>()[key]; }

		Reference get(Reference index) {
			if (index->is(Type::String)) {
				String key = index->as<String>();
				if (key == "type") return Value::Make(Names::Types[type]);

				if (is(Type::String)) {
					return Value::Make(int(as<String>().size()));
				} else if (is(Type::Array)) {
					if (key == "size") {
						return Value::Make(int(point<Array>()->size()));
					}
				} else if (is(Type::Table)) {
					if (key == "size") {
						return Value::Make(int(point<Table>()->size()));
					} else if (key == "keys") {
						Table* values = point<Table>(); Array output;
						for (const auto& [ name, variable ] : *values) output.push_back(Value::Make(name));
						return Value::Make(output);
					} else if (key == "values") {
						Table* values = point<Table>(); Array output;
						for (const auto& [ name, variable ] : *values) output.push_back(Value::Copy(variable));
						return Value::Make(output);
					}
				}
			}

			if (is(Type::Array)) {
				Array* array = point<Array>();

				if (index->is(Type::Range)) {
					Range range = index->as<Range>();
					
					Number from = range.get<0>();
					Number to = range.get<1>();

					if (from < 0) from = from + array->size();
					if (to < 0) to = to + array->size();

					Array output;
					if (from <= to) {
						for (int i = from; i <= to; i++) output.push_back(i < 0 || i > array->size() ? Empty() : Copy(array->at(i)));
					} else {
						for (int i = from; i >= to; i--) output.push_back(i < 0 || i > array->size() ? Empty() : Copy(array->at(i)));
					}

					return Value::Make(output);
				}

				int i = index->is(Type::String) ? std::stoi(index->as<String>()) : index->as<Number>();

				if (i < 0) i = i + array->size();
				if (i > array->size()) return Empty();
				if (!immutable && i == array->size()) array->push_back(Empty());

				return array->at(i);
			} else if (is(Type::Table) || is(Type::Block)) {
				Table* table = point<Table>();
				String key = index->as<String>();
				
				if (table->count(key) == 0) {
					if (immutable) return Empty();
					(*table)[key] = Empty();
				}

				return (*table)[key];
			}

			return Empty();
		}

		void set(Reference to, bool force=false) {
			if (!force && immutable) return;
			type = to->type;
			value = to->value;
			block = to->block;
			context = to->context;
		}

		void set(int index, Reference to, bool force=false) {
			if (!force && immutable) return;
			if (type != Type::Array) return;

			Array* array = point<Array>();
			if (index < 0 || index > array->size()) return;
			if (index == array->size()) array->push_back(Empty());
			array->at(index)->set(to);

			value = *array;
		}

		void set(string key, Reference to, bool force=false) {
			if (!force && immutable) return;
			if (type != Type::Table && type != Type::Block) return;
			
			Table* table = point<Table>();
			if (!table->count(key)) (*table)[key] = Empty();
			(*table)[key]->set(to);

			if (to->is(Type::Block) && !force) {
				(*table)[key]->container = Empty(Type::Table);
				(*table)[key]->container->value = value;
			} else {
				(*table)[key]->container = nullptr;
			}
			
			value = *table;
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
				if (type == Type::Block) return block == other->block;
			}

			return false;
		}

		Reference cast(Type to) {
			if (is(to)) return self();
			if (to == Type::String) return Value::Make(describe());

			if (is(Type::Void)) {
				if (to == Type::Boolean) {
					return Value::Make(false);
				} else if (to == Type::Number) {
					return Value::Make(0);
				}
			} else if (is(Type::Boolean)) {
				if (to == Type::Number) return Value::Make(as<Boolean>() ? 1 : 0);
			} else if (is(Type::Number)) {
				if (to == Type::Boolean) {
					return Value::Make(as<Number>() > 0);
				} else if (to == Type::Range) {
					return Value::Make(as<Number>(), as<Number>());
				}
			} else if (is(Type::Range)) {
				if (to == Type::Number) {
					return Value::Make(as<Number>() > 0);
				} else if (to == Type::Range) {
					return Value::Make(as<Number>(), as<Number>());
				}
			} else if (is(Type::String)) {
				if (to == Type::Boolean) {
					String value = as<String>();
					algorithm::to_lower(value);
					return Value::Make(value == "y" || value == "yes" || value == "true");
				} else if (to == Type::Number) {
					return Value::Make(std::stod(as<String>()));
				}
			}

			return Empty();
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
				
				Array* values = point<Array>();
				for (int i = 0; i < values->size(); i++) {
					if (i > 0) output += readable ? ",\n" : ", ";
					output += values->at(i)->describe(readable ? indent + 1 : -1);
				}

				return before + output + ( readable ? "\n" + Utils::Indent(indent) + "]" : " ]" );
			} else if (is(Type::Table)) {
				string output = readable ? "{\n" : "{ ";
				
				Table* values = point<Table>();
				int i = 0; for (const auto& [ name, variable ] : *values) {
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
		Reference container;
		Scope context;
		any value;
};

Reference Block::Bound(function<Reference(Array)> binding) { return Value::Locked(shared_ptr<Block>(new Block(binding))); }