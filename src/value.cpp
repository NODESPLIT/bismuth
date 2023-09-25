struct Argument {
	string name = "";
	vector<Lexer::Token> initial = {};
};

class Value {
	public:
		static unordered_map<Lexer::Token::Type, Type> Translation;

		static Reference Make() { return make_shared<Value>(); }

		template <class T> static Reference Make(T one) { return make_shared<Value>(one); }
		template <class T1, class T2> static Reference Make(T1 one, T2 two) { return make_shared<Value>(one, two); }

		static Reference Lock(Reference value) { value->immutable = true; return value; }
		
		class Static { public: static Reference True, False, Void; };

		static Reference Empty() { return Make(); }
		static Reference Empty(Type type) {
			if (type == Array) {
				return Make(ArrayValue{});
			} else if (type == Table) {
				return Make(TableValue{});
			}

			return Empty();
		}

		static Reference Copy(Reference from) {
			Reference copy = Empty();
			copy->set(from);
			return copy;
		}

		static Reference Dig(Reference value, ArrayValue path={}) {
			Reference result = value;
			for (auto &key : path) result = result->get(key);
			return result;
		}

		static Reference Bound(function<Reference(ArrayValue)> binding) {
			Reference value = Make();
			value->type = Block;
			value->binding = binding;
			return value;
		}

		bool immutable = false;

		Type type = Void;
		vector<vector<Lexer::Token>> inside = {};
		function<Reference(ArrayValue)> binding;
		vector<Argument> arguments;
		string description = "";
		Scope context;
		any value;

		bool is(Type type) { return this->type == type; }
		bool is(vector<Type> types) { for (auto &type : types) if (is(type)) return true; return false; }
		template <class T> T as() { return any_cast<T>(value); }

		string describe(NumberValue number) {
			if (!description.empty()) return description;

			std::stringstream stream;
	    stream.precision(numeric_limits<NumberValue>::digits10);
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
			string before = ( readable ? Log::Indent(indent) : "" ) + prefix;

			if (is(Boolean)) {
				return before + ( as<bool>() ? "true" : "false" );
			} else if (is(Number)) {
				return before + describe(as<NumberValue>());
			} else if (is(Range)) {
				RangeValue value = as<RangeValue>();
				return before + "( " + describe(std::get<0>(value)) + " -> " + describe(std::get<1>(value)) + " )";
			} else if (is(String)) {
				return before + "'" + as<string>() + "'";
			} else if (is(Array)) {
				string output = readable ? "[\n" : "[ ";
				
				for (int i = 0; i < length(); i++) {
					if (i > 0) output += readable ? ",\n" : ", ";
					output += get(i)->describe(readable ? indent + 1 : -1);
				}

				return before + output + ( readable ? "\n" + Log::Indent(indent) + "]" : " ]" );
			} else if (is(Table)) {
				string output = readable ? "{\n" : "{ ";
				
				TableValue values = as<TableValue&>();
				int i = 0; for (const auto& [ name, variable ] : values) {
					if (i > 0) output += readable ? ",\n" : ", "; i++;
					output += variable->describe(readable ? indent + 1 : -1, name + ": ");
				}

				return before + output + ( readable ? "\n" + Log::Indent(indent) + "}" : " }" );
			} else if (is(Block)) {
				return before + "'{ Block }'";
			}

			return before + "void";
		}

		Reference get(int index) { return as<ArrayValue>()[index]; }
		Reference get(string key) { return as<TableValue>()[key]; }

		int length() {
			if (is(String)) return as<string>().size();
			if (is(Array)) return as<ArrayValue>().size();
			if (is(Table)) return as<TableValue>().size();
			return 0;
		}

		Reference get(Reference key) {
			if (is(String) && key->is(Number)) return Make(as<string>().at((int)key->as<NumberValue>()));
			if (is(Array) && ( key->is(Number) || key->is(String) )) return as<ArrayValue>()[key->is(Number) ? (int)key->as<NumberValue>() : stod(key->as<string>())];
			if (is(Table)) return as<TableValue>()[ key->is(String) ? key->as<string>() : key->describe() ];
			return Empty();
		}

		void set(Reference to) {
			type = to->type;
			if (type == Block) context = to->context;
			inside = vector<vector<Lexer::Token>>(to->inside);
			description = to->description;
			context = to->context;
			arguments = to->arguments;
			binding = to->binding;
			value = to->value;
		}

		void set(int index, Reference to) {
			if (type != Array) return;
			ArrayValue array = as<ArrayValue&>();
			if (index < 0 || index > array.size()) return;
			if (index == array.size()) array.push_back(Empty());
			array[index]->set(to);
			value = array;
		}

		void set(string key, Reference to) {
			if (type != Table) return;
			TableValue table = as<TableValue&>();
			if (!table.count(key)) table[key] = Empty();
			table[key]->set(to);
			value = table;
		}

		void set(Reference field, Reference to) {
			if (!field) { set(to); return; }
			if (is(Array)) { set(field->is(Number) ? (int)field->as<NumberValue>() : stod(field->as<string>()), to); return; }
			if (is(Table)) set(field->is(String) ? field->as<string>() : field->describe(), to);
		}

		bool compare(Reference with) {
			if (with->type != type) {
				if (type == Boolean && with->type == Void) return !as<bool>();
				if (type == Void && with->type == Boolean) return !with->as<bool>();
				if (type == Number && with->type == Void) return as<NumberValue>() == 0;
				if (type == Void && with->type == Number) return with->as<NumberValue>() == 0;
				if (type == Boolean && with->type == Number) return as<bool>() == with->as<NumberValue>() > 0;
				if (type == Number && with->type == Boolean) return with->as<bool>() == as<NumberValue>() > 0;
				
				return false;
			} else {
				if (type == Void) return true;
				if (type == Boolean) return as<bool>() == with->as<bool>();
				if (type == Number) return as<NumberValue>() == with->as<NumberValue>();
				if (type == Range) return as<RangeValue>() == with->as<RangeValue>();
				if (type == String) return as<string>().compare(with->as<string>()) == 0;
			}

			return false;
		}

		bool truthy() {
			if (type == Void) return false;
			if (type == Boolean) return as<bool>();
			if (type == Number) return as<NumberValue>() > 0;
			return true;
		}

		bool falsey() { return !truthy(); }

		Value(){}

		Value(bool boolean) { this->type = Boolean; this->value = boolean; }
		Value(int number) { this->type = Number; this->value = NumberValue(number); }
		Value(float number) { this->type = Number; this->value = NumberValue(number); }
		Value(NumberValue number) { this->type = Number; this->value = number; }
		Value(NumberValue from, NumberValue to) { this->type = Range; this->value = RangeValue{ from, to }; }
		Value(string string) { this->type = String; this->value = string; }
		Value(ArrayValue array) { this->type = Array; this->value = array; }
		Value(TableValue table) { this->type = Table; this->value = table; }

		Value(Lexer::Token* token, Scope scope) {
			this->inside = token->inside;
			if (Value::Translation.count(token->type)) type = Value::Translation[token->type];

			if (type == Number) {
				description = token->contents;
				value = stod(token->contents);
			} else if (type == String) {
				value = token->contents;
			} else if (type == Block) {
				context = scope;

				Argument argument = Argument();
				bool defaulting = false;

				for (int i = 0; i <= inside[0].size(); i++) {
					if (argument.name.empty()) {
						if (inside[0][i].is(Lexer::Token::Type::Word)) {
							argument.name = inside[0][i].contents;
						}
					} else {
						if (inside[0][i].is(Lexer::Token::Type::Comma) || i == inside[0].size()) {
							arguments.push_back(argument);
							argument = Argument();
							defaulting = false;
						} else if (inside[0][i].is(Lexer::Token::Type::Operator, "=")) {
							defaulting = true;
						} else {
							argument.initial.push_back(inside[0][i]);
						}
					}
				}
			}
		}
};

unordered_map<Lexer::Token::Type, Type> Value::Translation = {
	{ Lexer::Token::Type::Number, Number },
	{ Lexer::Token::Type::String, String },
	{ Lexer::Token::Type::Table, Table },
	{ Lexer::Token::Type::Array, Array },
	{ Lexer::Token::Type::Block, Block }
};

Reference Value::Static::True = Value::Lock(Value::Make(true));
Reference Value::Static::False = Value::Lock(Value::Make(false));