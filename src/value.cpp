class Value {
	public:
		static unordered_map<string, Type> Translation;

		static Reference Make() { return make_shared<Value>(); }

		template <class T> static Reference Make(T one) { return make_shared<Value>(one); }
		template <class T1, class T2> static Reference Make(T1 one, T2 two) { return make_shared<Value>(one, two); }

		static Reference Lock(Reference value) { value->immutable = true; return value; }

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
		vector<vector<Lexer::Token>> inside;
		function<Reference(ArrayValue)> binding;
		Scope context;
		any value;

		bool is(Type type) { return this->type == type; }
		bool is(vector<Type> types) { for (auto &type : types) if (is(type)) return true; return false; }
		template <class T> T as() { return any_cast<T>(value); }

		string describe(int indent = -1, string prefix="") {
			bool readable = indent > -1;
			string before = ( readable ? Log::Indent(indent) : "" ) + prefix;

			if (is(Boolean)) {
				return before + ( as<bool>() ? "true" : "false" );
			} else if (is(Number)) {
				return before + to_string(as<NumberValue>());
			} else if (is(Range)) {
				RangeValue value = as<RangeValue>();
				return before + "[ " + to_string(std::get<0>(value)) + " to " + to_string(std::get<1>(value)) + " ]";
			} else if (is(String)) {
				return before + "'" + as<string>() + "'";
			} else if (is(Array)) {
				string output = readable ? "[\n" : "[ ";
				
				ArrayValue values = as<ArrayValue&>();
				for (int i = 0; i < values.size(); i++) {
					if (i > 0) output += readable ? ",\n" : ", ";
					output += values[i]->describe(readable ? indent + 1 : -1);
				}

				return before + output + ( readable ? "\n" + Log::Indent(indent) + "]" : " ]" );
			} else if (is(Table)) {
				string output = readable ? "{\n" : "{ ";
				
				TableValue values = as<TableValue&>();
				int i = 0;
				for (const auto& [ name, variable ] : values) {
					if (i > 0) output += readable ? ",\n" : ", "; i++;
					output += variable->describe(readable ? indent + 1 : -1, name + ": ");
				}

				return before + output + ( readable ? "\n" + Log::Indent(indent) + "}" : " }" );
			} else if (is(Block)) {
				return before + "{ Block }";
			}

			return before + "{ Void }";
		}

		Reference get(Reference key) {
			if (is(String) && key->is(Number)) return Make(as<string>().at((int)key->as<NumberValue>()));
			if (is(Array) && key->is(Number)) return as<ArrayValue>()[(int)key->as<NumberValue>()];
			if (is(Table) && key->is(String)) return as<TableValue>()[key->as<string>()];
			return Empty();
		}

		void set(Reference to) {
			type = to->type;
			if (type == Block) context = to->context;
			inside = vector<vector<Lexer::Token>>(to->inside);
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
			for (const auto& [ definition, type ] : Value::Translation) if (token->is(definition)) this->type = type;
			
			if (type == Number) {
				value = stod(token->contents);
			} else if (type == String) {
				value = token->contents;
			} else if (type == Block) {
				context = scope;
			}
		}
};

unordered_map<string, Type> Value::Translation = {
	{ "number", Number },
	{ "string", String },
	{ "table", Table },
	{ "array", Array },
	{ "block", Block }
};