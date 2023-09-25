class Parser {
	public:
		static void Log(Scope scope, int depth=0) {
			for (const auto& [ name, variable ] : *scope) {
				cout << variable->describe(depth, name + "[" + Typename[variable->type] + "]: ") << endl;
			}
		}

		static Lexer::Token* Read(vector<Lexer::Token>* tokens, int at) {
			if (at >= tokens->size() || at < 0) return nullptr;
			return &(*tokens)[at];
		}

		static void Setup(Scope scope, string path) { Setup(scope, path, Value::Empty()); }
		static void Setup(Scope scope, string path, Reference input) {
			(*scope)["@"] = Value::Empty(Table);
			(*scope)["@"]->set("path", Value::Lock(Value::Make(path)));
			(*scope)["@"]->set("in", input ? Value::Lock(Value::Copy(input)) : Value::Empty());
		}

		Scope global = make_shared<ScopeValue>();
		Scope environment = make_shared<ScopeValue>();
		Scope current = make_shared<ScopeValue>();

		Reference result = Value::Empty();
		bool returned = false;

		Parser() {
			Environment::Bind(this);
			Setup(global, "./");
		}

		Parser(std::string path, bool verbose=false) {
			if (verbose) cout << Stamp << endl;
			Environment::Bind(this);
			path = Search::Path(path);
			Setup(global, path);
			result = run(Load(path), verbose);
		}

		Scope branched(Scope origin) {
			Scope scope = make_shared<ScopeValue>();
			for (const auto& [ name, variable ] : *origin) (*scope)[name] = variable;
			return scope;
		}

		ArrayValue resolveList(vector<Lexer::Token>* tokens, Scope scope) {
			ArrayValue output; if (tokens->size() == 0) return output;
			vector<Lexer::Token> step = {};

			for (int i = 0; i <= (*tokens).size(); i++) {
				if (i == (*tokens).size() || ( (*tokens)[i].is(Lexer::Token::Type::Comma) || (*tokens)[i].is(Lexer::Token::Type::End) )) {
					if (step.size() > 0) output.push_back(Value::Copy(resolve(&step, scope)));
					step.clear();
				} else {
					if (!(*tokens)[i].is(Lexer::Token::Type::Comma) || (*tokens)[i].is(Lexer::Token::Type::End)) step.push_back((*tokens)[i]);
				}
			}

			return output;
		};

		TableValue resolveTable(vector<Lexer::Token>* tokens, Scope scope) {
			TableValue output; if (tokens->size() == 0) return output;
			vector<Lexer::Token> step = {};

			string name = "";
			bool named = false;

			for (int i = 0; i <= (*tokens).size(); i++) {
				if (named) {
					if (step.size() > 0 && i == (*tokens).size() || ( (*tokens)[i].is(Lexer::Token::Type::Comma) || (*tokens)[i].is(Lexer::Token::Type::End) )) {
						if (step.size() > 0) output[name] = Value::Copy(resolve(&step, scope));
						step.clear(); name = ""; named = false;
					} else {
						if (!(*tokens)[i].is(Lexer::Token::Type::Comma) || (*tokens)[i].is(Lexer::Token::Type::End)) step.push_back((*tokens)[i]);
					}
				} else {
					if ((*tokens)[i].is(Lexer::Token::Type::Word)) name = (*tokens)[i].contents;
					if ((*tokens)[i].is(Lexer::Token::Type::Operator, ":")) named = true;
				}
			}

			return output;
		};

		Reference resolveIf(vector<vector<Lexer::Token>>* inside, Scope scope) {
			for (int i = 0; i < inside->size(); i += 2) if (resolve(&(*inside)[i], scope)->truthy()) return run(&(*inside)[i + 1], scope, false);
			return Value::Empty();
		}

		Reference call(Reference block) { return call(block, {}, current, true); }
		Reference call(Reference block, ArrayValue values) { return call(block, values, current, true); }
		Reference call(Reference block, ArrayValue values, Scope scope, bool branch=false) {
			if (block->binding) return block->binding(values);
			if (block->context) scope = block->context;
			if (branch) scope = branched(scope);

			for (int i = 0; i < block->arguments.size(); i++) {
				if (i < values.size()) {
					(*scope)[block->arguments[i].name] = Value::Copy(values[i]);
				} else {
					(*scope)[block->arguments[i].name] = block->arguments[i].initial.empty() ? Value::Empty() : resolve(&block->arguments[i].initial, scope);
				}
			}

			return run(&block->inside[1], scope);
		}

		Reference resolve(vector<Lexer::Token>* tokens, Scope scope, int position=0, Reference target=nullptr) {
			current = scope;

			Lexer::Token* left = Read(tokens, position);
			if (!target && !left) return Value::Empty();

			Reference container = nullptr;
			Reference index = nullptr;

			if (!target && left->is(Lexer::Token::Type::Word)) {
				switch (left->symbol) {
					case Lexer::Token::Symbol::Return:
						returned = true;
						return resolve(tokens, scope, position + 1);
					case Lexer::Token::Symbol::True: target = Value::Make(true); break;
					case Lexer::Token::Symbol::False: target = Value::Make(false); break;
					case Lexer::Token::Symbol::Void: target = Value::Empty(); break;
					default:
						if (scope->count(left->contents)) target = (*scope)[left->contents];
						if (environment->count(left->contents)) { target = (*environment)[left->contents]; }
				}
			}

			if (!target && left->is(Lexer::Token::Type::If)) {
				target = resolveIf(&left->inside, scope);
				if (returned) return target;
			}

			if (!target && left->is(Lexer::Token::Type::Brackets)) target = resolve(&left->inside[0], scope);
			if (!target && left->is(Lexer::Token::Type::Array)) target = Value::Make(resolveList(&left->inside[0], scope));
			if (!target && left->is(Lexer::Token::Type::Table)) target = Value::Make(resolveTable(&left->inside[0], scope));
			
			if (!target && left->is(Lexer::Token::Type::Index)) {
				container = resolve(&left->inside[0], scope); for (int i = 1; i < left->inside.size() - 1; i++) container = container->get(resolve(&left->inside[i], scope));
				index = resolve(&left->inside[left->inside.size() - 1], scope);
				target = container->get(index);
			}

			if (!target && left->is(Lexer::Token::Type::Call)) {
				target = resolve(&left->inside[0], scope);
				for (int i = 1; i < left->inside.size(); i++) target = Value::Copy(call(Value::Copy(target), resolveList(&left->inside[i], scope), scope, true));
			}

			if (!target) {
				if (!left->is(Lexer::Token::Type::Word)) {
					target = make_shared<Value>(left, scope);
				} else {
					(*scope)[left->contents] = Value::Empty();
					target = (*scope)[left->contents];
				}
			}

			Lexer::Token* middle = Read(tokens, position + 1);
			if (!middle) return target;
			
			if (!target->immutable && middle->is(Lexer::Token::Type::Operator)) {
				if (Operator::List[middle->contents].category == Operator::Category::Assignment) {
					Reference right = resolve(tokens, scope, position + 2);

					if (!middle->is(Lexer::Token::Type::Operator, "=")) {
						Operation operation = findOperator(string{ middle->contents.at(0) }, target, right);
						right = operation ? operation(target, right, this, scope) : Value::Empty();
					}

					if (container) {
						container->set(index, right);
						return container->get(index);
					}

					target->set(right);
					return target;
				} else if (Operator::List[middle->contents].pairing == Operator::Pairing::Unary) {
					Operation operation = findOperator(middle->contents, target, Value::Empty());
					Reference right = operation ? operation(target, Value::Empty(), this, scope) : Value::Empty();

					if (container) {
						container->set(index, right);
						return container->get(index);
					}

					target->set(right);
					return target;
				}
			}

			if (middle->is(Lexer::Token::Type::Operator) && Operator::List.count(middle->contents)) {
				Lexer::Token* token = Read(tokens, position + 2);
				if (token) {
					Reference right = resolve(token, scope);
					int gap = 2;

					Lexer::Token* nextOperator = Read(tokens, position + 3);
					if (nextOperator->is(Lexer::Token::Type::Operator) && Operator::List[nextOperator->contents].priority > Operator::List[middle->contents].priority) {
						Reference nextRight = resolve(Read(tokens, position + 4), scope);
						Operation operation = findOperator(nextOperator->contents, right, nextRight);
						if (operation) { right = operation(right, nextRight, this, scope); gap = 4; }
					}

					Operation operation = findOperator(middle->contents, target, right);
					if (operation) return resolve(tokens, scope, position + gap, operation(target, right, this, scope));
				}
			}

			return Value::Empty();
		}

		Reference resolve(Lexer::Token* token, Scope scope, int position=0, Reference target=nullptr) {
			vector<Lexer::Token> tokens = {}; tokens.push_back(*token);
			return resolve(&tokens, scope, position, target);
		}

		Operation findOperator(string op, Reference left, Reference right) {
			if (Operator::List[op].operations.count(left->type) && Operator::List[op].operations[left->type].count(right->type)) {
				return Operator::List[op].operations[left->type][right->type];
			} else if (Operator::List[op].operations.count(left->type) && Operator::List[op].operations[left->type].count(Any)) {
				return Operator::List[op].operations[left->type][Any];
			} else if (Operator::List[op].operations.count(Any) && Operator::List[op].operations[Any].count(right->type)) {
				return Operator::List[op].operations[Any][right->type];
			} else if (Operator::List[op].operations.count(Any) && Operator::List[op].operations[Any].count(Any)) {
				return Operator::List[op].operations[Any][Any];
			}

			return nullptr;
		}

		Reference run(vector<Lexer::Token>* tokens, Scope scope=nullptr, bool returns=true) {
			if (!scope) scope = global;

			Reference result = Value::Empty();
			vector<Lexer::Token> step = {};
			
			for (int i = 0; i <= (*tokens).size(); i++) {
				if (i == (*tokens).size() || (*tokens)[i].is(Lexer::Token::Type::End)) {
					if (step.size() > 0) {
						result = resolve(&step, scope); step.clear();
						if (returned) { if (returns) returned = false; break; }
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

			vector<Lexer::Token> tokens = {};
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
			filesystem::path base((*scope)["@"]->get("path")->as<string>()); base.remove_filename();
			return base / path;
		}
};