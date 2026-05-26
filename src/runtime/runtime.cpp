Runtime* Instance;

#if BOOST_VERSION >= 108400
vector<std::shared_ptr<Bismuth::API>> Crystals;
#else
vector<boost::shared_ptr<Bismuth::API>> Crystals;
#endif

void Runtime::Log(Scope scope, int depth) {
	bool first = true;
	for (const auto& [ name, variable ] : *scope) {
		if (first) { first = false; } else { cout << endl; }
		cout << variable->describe(depth, name + "[" + Names::Types[variable->type] + "]: ") << endl;
	}
}

Runtime::Runtime() { Bindings::Bind(this); init(global); }
Runtime::Runtime(string path, Reference in) { Bindings::Bind(this); result = load(path, in, global); }

Reference Runtime::load(string path, Reference in, Scope scope) {
	int initialTests = tests.size();
	path = Search::Path(path);

	if (!scope) scope = global;
	init(scope, path, in);

	ifstream stream = ifstream(path);
	
	if (!stream.good()) return Value::Empty();
	string bismuth((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

	if (Bismuth::Verbose) cout << Utils::Prefix(bismuth, "│   ", "\n┌┄┈ Bismuth [" + path + "]\n│\n│\n");

	Reference result = interpret(bismuth);
	scopes.pop();

	int newTests = tests.size() - initialTests;

	struct {
		string file = "";
		std::filesystem::path path;
		string content = "";
	} doc;

	if (newTests > 0) {
		if (Bismuth::Testing || Bismuth::Document) {
			if (Bismuth::Testing) {
				cout << "\n\033[37m[ " << path << " ]\033[0m" << endl;
				cout << Utils::Boxed(Bismuth::Header, 68, 1, true, false) << endl << "\033[37m┊\033[0m" << endl;
			} else if (Bismuth::Document) {
				doc.file = std::filesystem::path(path).stem().stem().string() + ".md";
				doc.path = std::filesystem::path(Bismuth::DocumentPath) / std::filesystem::path(path).remove_filename();
				std::filesystem::create_directories(doc.path);

				doc.content += "# ";
				doc.content += Bismuth::Header;
				doc.content += "\n\n";
			}

			bool passed = true;
			int total = 0;

			for (int i = initialTests; i < tests.size(); i++) {
				Test test = tests[i];

				if (test.passed) {
					total++;
				} else {
					passed = false;
				}

				if (Bismuth::Testing) {
					cout << ( test.passed ? "\033[92m" : "\033[91m" );
					cout << ( i == 0 ? "┎" : i == tests.size() - 1 ? "┖" : "┠" ) << " " << ( test.passed ? "🟢" : "🔴" ) << " " << test.description << endl;
					cout << "\033[0m";
				} else if (Bismuth::Document) {
					doc.content += "---\n\n";
					doc.content += "##### " + test.description;
					doc.content += "\n\n###### Example:\n";
					doc.content += "```\n";
					string method = test.method.substr(test.method.find("]{") + 2);
					method = method.substr(0, method.size() - 1);
					int lines = std::count(method.begin(), method.end(), '\n');
					doc.content += lines == 0 ? Utils::Trim(method) : method;
					doc.content += "\n```\n";
					doc.content += "```\n=> ";
					doc.content += test.result;
					doc.content += "\n```\n\n";
				}
			}

			if (Bismuth::Testing) {
				cout << ( passed ? "\033[92m" : "\033[91m" );
				cout << "┊" << endl << "└ " << ( passed ? "🟢" : "🔴" ) << " [\033[1m" << ( passed ? "SUCCESS" : "FAILURE" ) << "\033[0m" << ( passed ? "\033[92m" : "\033[91m" ) << "] " << total << " of " << tests.size() << " passed" << endl << endl;
				cout << "\033[0m";
			} else if (Bismuth::Document) {
				std::ofstream file(doc.path / doc.file);
				file << doc.content.c_str();
				file.close();
			}

			result = Value::Make(passed);
		}
	}

	return result;
}

Reference Runtime::import(string path, Reference in) {
	std::filesystem::path base((*scopes.top())[Symbol::Context]->get("path")->as<string>()); base.remove_filename();

	if (path.ends_with(".crystal")) {
		
		#if BOOST_VERSION >= 108400
		Crystals.push_back(boost::dll::import_symbol<Bismuth::API>((base / path).string(), "crystal"));
		#else
		Crystals.push_back(boost::dll::import_alias<Bismuth::API>((base / path).string(), "crystal"));
		#endif

		return Crystals.back()->construct(Instance);
	}

	Scope subscope = branched();
	return load(base / path, in, subscope);
}

Reference Runtime::interpret(string bismuth) {
	vector<Token> tokens;
	Analyse(bismuth, &tokens);

	if (Bismuth::Verbose) cout << Utils::Prefixed([&]{ Token::Log(&tokens); }, "│   ", "\n│\n│\n└┄┈\n┊\n┌┄┈ Tokens\n│\n│\n");

	return interpret(&tokens);
}

Reference Runtime::interpret(vector<Token>* tokens) {
	vector<Node> tree;
	Parse(tokens, &tree);

	if (Bismuth::Verbose) cout << Utils::Prefixed([&]{ Node::Log(&tree); }, "│   ", "\n│\n└┄┈\n┊\n┌┄┈ Abstract Syntax Tree\n│\n│\n");

	return interpret(&tree);
}

Reference Runtime::interpret(vector<Node>* tree) {
	vector<Annotated> instructions;
	Machine::Compile(this, tree, &instructions);

	if (Bismuth::Verbose) cout << Utils::Prefixed([&, this]{ Machine::Log(this, &instructions); }, "│  ", "\n│\n└┄┈\n┊\n┌┄┈ Instructions\n│\n│\n");

	Instance = this;
	Reference result = run(instructions);
	
	if (Bismuth::Verbose) {
		cout << Utils::Prefix("=> " + result->describe(), "│   ", "\n│\n└┄┈\n┊\n┌┄┈ Result\n│\n│\n");
		cout << Utils::Prefixed([&,this]{ Log(global); }, "│   ", "\n│\n│\n└┄┈\n┊\n┌┄┈ Globals\n│\n│\n");
		cout << endl << "│" << endl << "└┄┈" << endl << endl;
	}

	return result;
}

void Runtime::init(Scope scope, string path, Reference in) {
	(*scope)[Symbol::Context] = Value::Lock(Value::Empty(Type::Table));
	
	(*scope)[Symbol::Context]->SET("path", Value::Lock(Value::Make(path)));
	(*scope)[Symbol::Context]->SET("in", in ? Value::Lock(Value::Copy(in)) : Value::Empty());

	(*scope)[Symbol::Context]->SET(
		"test",
		Block::Bound(
			[=, this](Array arguments) {
				if (!Bismuth::Testing && !Bismuth::Document) return Value::Make(false);

				string description = arguments[0]->as<String>();
				Reference result = call(arguments[1]);

				bool pass = false;

				if (arguments[2]->is(Type::Block)) {
					pass = call(arguments[2], { Value::Copy(result) })->isTrue();
				} else {
					pass = result->equals(arguments[2]);
				}

				tests.push_back({ description, pass, arguments[1]->describe(0), arguments[2]->describe(0) });
				return Value::Make(pass);
			}
		)
	);

	scopes.push(scope);
}

Scope Runtime::branched(Scope origin, Table initial) {
	if (origin) {
		Scope branch = boost::make_shared<Table>(*origin);
		(*branch)[Symbol::Context] = Value::Empty(Type::Table);
		for (const auto& [ name, variable ] : *((*origin)[Symbol::Context]->point<Table>())) (*branch)[Symbol::Context]->SET(name, Value::Copy(variable));
		return branch;
	}

	return Scope(new Table());
}