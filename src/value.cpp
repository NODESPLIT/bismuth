class Value {
	public:
		class Static { public: static Reference True, False, Void; };
		static unordered_map<Lexer::Token::Type, Type> Translation;

		static Reference Make() { return make_shared<Value>(); }
		template <class T> static Reference Make(T one) { return make_shared<Value>(one); }
		template <class T1, class T2> static Reference Make(T1 one, T2 two) { return make_shared<Value>(one, two); }

		static Reference Empty() { return Make(); }
		static Reference Empty(Type type) {
			if (type == Type::Array) {
				return Make(ArrayValue{});
			} else if (type == Type::Table) {
				return Make(TableValue{});
			}

			return Empty();
		}

		static Reference Lock(Reference value) { value->immutable = true; return value; }
		static Reference Copy(Reference from) { Reference copy = Empty(); copy->set(from); return copy; }

		static Reference Bound(function<Reference(ArrayValue)> binding) { return Make(make_shared<Block>(binding)); }

		bool immutable = false;
		Type type = Type::Void;
		shared_ptr<Block> block;
		any value;

		bool is(Type type) { return this->type == type; }
		bool is(vector<Type> types) { for (auto &type : types) if (is(type)) return true; return false; }
		template <class T> T as() { return any_cast<T>(value); }

		string describe(NumberValue number) {
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

			if (is(Type::Boolean)) {
				return before + ( as<bool>() ? "true" : "false" );
			} else if (is(Type::Number)) {
				return before + describe(as<NumberValue>());
			} else if (is(Type::Range)) {
				RangeValue value = as<RangeValue>();
				return before + "( " + describe(std::get<0>(value)) + " -> " + describe(std::get<1>(value)) + " )";
			} else if (is(Type::String)) {
				return before + "'" + as<string>() + "'";
			} else if (is(Type::Array)) {
				string output = readable ? "[\n" : "[ ";
				
				for (int i = 0; i < length(); i++) {
					if (i > 0) output += readable ? ",\n" : ", ";
					output += get(i)->describe(readable ? indent + 1 : -1);
				}

				return before + output + ( readable ? "\n" + Log::Indent(indent) + "]" : " ]" );
			} else if (is(Type::Table)) {
				string output = readable ? "{\n" : "{ ";
				
				TableValue values = as<TableValue&>();
				int i = 0; for (const auto& [ name, variable ] : values) {
					if (i > 0) output += readable ? ",\n" : ", "; i++;
					output += variable->describe(readable ? indent + 1 : -1, name + ": ");
				}

				return before + output + ( readable ? "\n" + Log::Indent(indent) + "}" : " }" );
			} else if (is(Type::Block)) {
				return before + "'{ Block }'";
			}

			return before + "void";
		}

		Reference get(int index) { return as<ArrayValue>()[index]; }
		Reference get(string key) { return as<TableValue>()[key]; }

		int length() {
			if (is(Type::String)) return as<string>().size();
			if (is(Type::Array)) return as<ArrayValue>().size();
			if (is(Type::Table)) return as<TableValue>().size();
			return 0;
		}

		Reference get(Reference key) {
			if (is(Type::String) && key->is(Type::Number)) return Make(as<string>().at((int)key->as<NumberValue>()));
			if (is(Type::Array) && ( key->is(Type::Number) || key->is(Type::String) )) return as<ArrayValue>()[key->is(Type::Number) ? (int)key->as<NumberValue>() : stod(key->as<string>())];
			if (is(Type::Table)) return as<TableValue>()[ key->is(Type::String) ? key->as<string>() : key->describe() ];
			return Empty();
		}

		void set(Reference to) {
			type = to->type;
			value = to->value;
			block = to->block;
		}

		void set(int index, Reference to) {
			if (type != Type::Array) return;
			ArrayValue array = as<ArrayValue&>();
			if (index < 0 || index > array.size()) return;
			if (index == array.size()) array.push_back(Empty());
			array[index]->set(to);
			value = array;
		}

		void set(string key, Reference to) {
			if (type != Type::Table) return;
			TableValue table = as<TableValue&>();
			if (!table.count(key)) table[key] = Empty();
			table[key]->set(to);
			value = table;
		}

		void set(Reference field, Reference to) {
			if (!field) { set(to); return; }
			if (is(Type::Array)) { set(field->is(Type::Number) ? (int)field->as<NumberValue>() : stod(field->as<string>()), to); return; }
			if (is(Type::Table)) set(field->is(Type::String) ? field->as<string>() : field->describe(), to);
		}

		bool compare(Reference with) {
			if (with->type != type) {
				if (type == Type::Boolean && with->type == Type::Void) return !as<bool>();
				if (type == Type::Void && with->type == Type::Boolean) return !with->as<bool>();
				if (type == Type::Number && with->type == Type::Void) return as<NumberValue>() == 0;
				if (type == Type::Void && with->type == Type::Number) return with->as<NumberValue>() == 0;
				if (type == Type::Boolean && with->type == Type::Number) return as<bool>() == with->as<NumberValue>() > 0;
				if (type == Type::Number && with->type == Type::Boolean) return with->as<bool>() == as<NumberValue>() > 0;
				
				return false;
			} else {
				if (type == Type::Void) return true;
				if (type == Type::Boolean) return as<bool>() == with->as<bool>();
				if (type == Type::Number) return as<NumberValue>() == with->as<NumberValue>();
				if (type == Type::Range) return as<RangeValue>() == with->as<RangeValue>();
				if (type == Type::String) return as<string>().compare(with->as<string>()) == 0;
			}

			return false;
		}

		bool truthy() {
			if (type == Type::Void) return false;
			if (type == Type::Boolean) return as<bool>();
			if (type == Type::Number) return as<NumberValue>() > 0;
			return true;
		}

		bool falsey() { return !truthy(); }

		Value(){}

		Value(bool boolean) { type = Type::Boolean; value = boolean; }
		Value(int number) { type = Type::Number; value = NumberValue(number); }
		Value(float number) { type = Type::Number; value = NumberValue(number); }
		Value(NumberValue number) { type = Type::Number; value = number; }
		Value(NumberValue from, NumberValue to) { type = Type::Range; value = RangeValue{ from, to }; }
		Value(string string) { type = Type::String; value = string; }
		Value(ArrayValue array) { type = Type::Array; value = array; }
		Value(TableValue table) { type = Type::Table; value = table; }

		Value(shared_ptr<Block> block) {
			type = Type::Block;
			this->block = block;
		}

		Value(Lexer::Token* token, Scope scope) {
			if (Value::Translation.count(token->type)) type = Value::Translation[token->type];

			if (type == Type::Number) {
				value = stod(token->contents);
			} else if (type == Type::String) {
				value = token->contents;
			} else if (type == Type::Block) {
				block = make_shared<Block>(token, scope);
			}
		}
};

unordered_map<Lexer::Token::Type, Type> Value::Translation = {
	{ Lexer::Token::Type::Number, Type::Number },
	{ Lexer::Token::Type::String, Type::String },
	{ Lexer::Token::Type::Table, Type::Table },
	{ Lexer::Token::Type::Array, Type::Array },
	{ Lexer::Token::Type::Block, Type::Block }
};

Reference Value::Static::True = Value::Lock(Value::Make(true));
Reference Value::Static::False = Value::Lock(Value::Make(false));
Reference Value::Static::Void = Value::Lock(Value::Make());