class Parser {
	public:
		static constexpr const char* Name = "Bismuth";
		static constexpr const char* Version = "0.0.1";
		static const vector<string> Extensions;

		static string Load(string path) {
			ifstream stream = ifstream(path);

			if (!stream.good()) {
				for (int i = 0; i < Extensions.size(); i++) {
					stream = ifstream(path + "." + Extensions[i]);
					if (stream.good()) break;
				}
			}

			if (!stream.good()) return "";
			string script((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

			return script;
		}
		
		static void Log(Scope scope, int depth=0) {
			for (const auto& [ name, variable ] : *scope) {
				cout << variable->describe(depth, name + "[" + Typename[variable->type] + "]: ") << endl;
			}
		}

		static Lexer::Token* Read(vector<Lexer::Token>* tokens, int at) {
			if (at >= tokens->size() || at < 0) return nullptr;
			return &(*tokens)[at];
		}

		Scope global = make_shared<ScopeValue>();
		Scope environment = make_shared<ScopeValue>();
		Scope current = make_shared<ScopeValue>();

		Parser() {
			Environment::Bind(this);
			(*global)["PATH"] = Value::Lock(Value::Make(string("./")));
		}

		Parser(std::string path, bool verbose=false) {
			Environment::Bind(this);
			(*global)["PATH"] = Value::Lock(Value::Make(path));
			run(Parser::Load(path), verbose);
		}

		Scope branched(Scope origin) {
			Scope scope = make_shared<ScopeValue>();
			for (const auto& [ name, variable ] : *origin) (*scope)[name] = variable;
			return scope;
		}

		ArrayValue resolveList(vector<Lexer::Token>* tokens, Scope scope) {
			ArrayValue output; if (tokens->size() == 0) return output;
			vector<Lexer::Token> step;

			for (int i = 0; i <= (*tokens).size(); i++) {
				if (i == (*tokens).size() || ( (*tokens)[i].is("comma") || (*tokens)[i].is("end") )) {
					if (step.size() > 0) output.push_back(Value::Copy(resolve(&step, scope)));
					step.clear();
				} else {
					if (!(*tokens)[i].is("comma") || (*tokens)[i].is("end")) step.push_back((*tokens)[i]);
				}
			}

			return output;
		};

		TableValue resolveTable(vector<Lexer::Token>* tokens, Scope scope) {
			TableValue output; if (tokens->size() == 0) return output;
			vector<Lexer::Token> step;

			string name = "";
			bool named = false;

			for (int i = 0; i <= (*tokens).size(); i++) {
				if (named) {
					if (step.size() > 0 && i == (*tokens).size() || ( (*tokens)[i].is("comma") || (*tokens)[i].is("end") )) {
						if (step.size() > 0) output[name] = Value::Copy(resolve(&step, scope));
						step.clear(); name = ""; named = false;
					} else {
						if (!(*tokens)[i].is("comma") || (*tokens)[i].is("end")) step.push_back((*tokens)[i]);
					}
				} else {
					if ((*tokens)[i].is("word")) name = (*tokens)[i].contents;
					if ((*tokens)[i].is("operator", ":")) named = true;
				}
			}

			return output;
		};

		Reference call(Reference block, ArrayValue values, Scope scope, bool branch=false) {
			if (block->context) scope = block->context;
			if (branch) scope = branched(scope);

			if (block->binding) return block->binding(values);

			vector<string> arguments;
			for (auto &token : block->inside[0]) if (token.is("word")) arguments.push_back(token.contents);
			for (int i = 0; i < values.size(); i++) if (i < arguments.size()) (*scope)[arguments[i]] = Value::Copy(values[i]);

			return run(&block->inside[1], scope);
		}

		Reference resolve(vector<Lexer::Token>* tokens, Scope scope, int position=0, Reference target=nullptr) {
			current = scope;

			Lexer::Token* left = Read(tokens, position);
			if (!target && !left) return Value::Empty();

			if (!target && left->is("brackets")) target = resolve(&left->inside[0], scope);
			if (!target && left->is("word", "true")) target = Value::Make(true);
			if (!target && left->is("word", "false")) target = Value::Make(false);
			if (!target && left->is("word") && scope->count(left->contents)) target = (*scope)[left->contents];
			if (!target && left->is("word") && environment->count(left->contents)) { target = (*environment)[left->contents]; }
			if (!target && left->is("array")) target = Value::Make(resolveList(&left->inside[0], scope));
			if (!target && left->is("table")) target = Value::Make(resolveTable(&left->inside[0], scope));
			if (!target && left->is("range")) {
				ArrayValue range = resolveList(&left->inside[0], scope);
				target = Value::Make(range[0]->as<NumberValue>(), range[1]->as<NumberValue>());
			}
			
			if (!target && left->is("index")) {
				Reference value = resolve(&left->inside[0], scope);
				ArrayValue path;
				for (int i = 1; i < left->inside.size(); i++) path.push_back(resolve(&left->inside[i], scope));
				target = Value::Dig(value, path);
			}

			if (!target && left->is("call")) {
				Reference value = resolve(&left->inside[0], scope);
				for (int i = 1; i < left->inside.size(); i++) value = call(value, resolveList(&left->inside[i], scope), scope, true);
				return value;
			}

			if (!target) {
				if (!left->is("word")) {
					target = make_shared<Value>(left, scope);
				} else {
					(*scope)[left->contents] = Value::Empty();
					target = (*scope)[left->contents];
				}
			}

			Lexer::Token* middle = Read(tokens, position + 1);
			if (!middle) return target;
			
			if (middle->is("operator", "=") && !target->immutable) {
				target->set(resolve(tokens, scope, position + 2));
				return target;
			}

			if (middle->is("operator") && Operator::List.count(middle->contents)) {
				Lexer::Token* token = Read(tokens, position + 2);
				if (token) {
					Reference right = resolve(token, scope);
					int gap = 2;

					Lexer::Token* nextOperator = Read(tokens, position + 3);
					if (nextOperator->is("operator") && Operator::List[nextOperator->contents].priority > Operator::List[middle->contents].priority) {
						Reference nextRight = resolve(Read(tokens, position + 4), scope);

						if (Operator::List[nextOperator->contents].operations.count(right->type) && Operator::List[middle->contents].operations[right->type].count(nextRight->type)) {
							right = Operator::List[nextOperator->contents].operations[right->type][nextRight->type](right, nextRight, this, scope);
							gap = 4;
						}
					}

					if (Operator::List[middle->contents].operations.count(target->type) && Operator::List[middle->contents].operations[target->type].count(right->type)) {
						return resolve(
							tokens, scope, position + gap,
							Operator::List[middle->contents].operations[target->type][right->type](target, right, this, scope)
						);
					}
				}
			}

			return Value::Empty();
		}

		Reference resolve(Lexer::Token* token, Scope scope, int position=0, Reference target=nullptr) {
			vector<Lexer::Token> tokens; tokens.push_back(*token);
			return resolve(&tokens, scope, position, target);
		}

		Reference run(vector<Lexer::Token>* tokens, Scope scope=nullptr) {
			if (!scope) scope = global;

			Reference result = Value::Empty();
			vector<Lexer::Token> step;
			
			for (int i = 0; i <= (*tokens).size(); i++) {
				if (i == (*tokens).size() || (*tokens)[i].is("end")) {
					if (step.size() > 0) {
						result = resolve(&step, scope);
						step.clear();
					}
				} else {
					step.push_back((*tokens)[i]);
				}
			}

			return result;
		}

		Reference run(string script, bool verbose=false, Scope scope=nullptr) {
			if (verbose) {
				cout << "\n\n--- Script ---" << endl;
				cout << endl << script << endl;
			}

			vector<Lexer::Token> tokens;
			Lexer::Tokens(script, &tokens);

			if (verbose) {
				cout << "\n\n--- Tokens ---\n" << endl;
				Lexer::Token::Log(&tokens);
				cout << "\n--- Parsing ---" << endl;
			}

			Reference result = run(&tokens, scope);
			
			if (verbose) {
				cout << "\nResult: " << result->describe(0) << endl << endl;
				cout << "\n--- Globals ---\n" << endl;
				Log(global); cout << endl << endl;
			}

			return result;
		}

		string subpath(string path, Scope scope=nullptr) {
			if (!scope) scope = global;
			filesystem::path base((*scope)["PATH"]->as<string>()); base.remove_filename();
			return base / path;
		}
};

unordered_map<string, Operator> Operator::List = {
	{
		":",
		Operator {
			3,
			Operator::Pairing::Binary,
			Operator::Category::Inside,
			{
				{
					Number,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								NumberValue amount = left->as<NumberValue>();

								ArrayValue output;
								if (amount >= 0) {
									for (int i = 0; i < amount; i++) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								} else {
									for (int i = amount + 1; i >= 0; i--) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								}

								return Value::Make(output);
							}
						}
					}
				},
				{
					Range,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								RangeValue range = left->as<RangeValue>();

								NumberValue from = get<0>(range);
								NumberValue to = get<1>(range);
								
								ArrayValue output;
								if (from <= to) {
									for (int i = from; i <= to; i++) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								} else {
									for (int i = from; i >= to; i--) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								}

								return Value::Make(output);
							}
						}
					}
				},
				{
					Array,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								ArrayValue array = left->as<ArrayValue>();

								ArrayValue output;
								for (int i = 0; i < array.size(); i++) output.push_back(parser->call(right, { array[i], Value::Make(i) }, scope));

								return Value::Make(output);
							}
						}
					}
				},
				{
					Table,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								TableValue table = left->as<TableValue>();
								
								TableValue output;
								for (const auto& [ name, variable ] : table) output[name] = parser->call(right, { variable, Value::Make(name) }, scope);

								return Value::Make(output);
							}
						}
					}
				},
				{
					String,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								string value = left->as<string>();
								
								string output = "";
								for (int i = 0; i < value.size(); i++) {
									string character({ value.at(i) });
									Reference result = parser->call(right, { Value::Make(character), Value::Make(i) }, scope);
									if (result->is(String)) output += result->as<string>();
								}

								return Value::Make(output);
							}
						}
					}
				}
			}
		}
	},
	{
		"=",
		Operator {
			3,
			Operator::Pairing::Binary,
			Operator::Category::Assignment,
			{}
		}
	},
	{
		"+",
		Operator {
			1,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() + right->as<NumberValue>());
						} }
					}
				},
				{
					String,
					{
						{ String, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<string>() + right->as<string>());
						} }
					}
				}
			}
		}
	},
	{
		"-",
		Operator {
			1,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() - right->as<NumberValue>());
						} }
					}
				}
			}
		}
	},
	{
		"*",
		Operator {
			2,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() * right->as<NumberValue>());
						} }
					}
				}
			}
		}
	},
	{
		"/",
		Operator {
			2,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() / right->as<NumberValue>());
						} }
					}
				}
			}
		}
	},
	{
		"%",
		Operator {
			1,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(fmod(left->as<NumberValue>(), right->as<NumberValue>()));
						} }
					}
				}
			}
		}
	}
};