Runtime* Instance;

#if BOOST_VERSION >= 108400
vector<std::shared_ptr<Bismuth::API>> Crystals;
#else
vector<boost::shared_ptr<Bismuth::API>> Crystals;
#endif

void Runtime::Log(Scope scope, int depth) {
	for (const auto& [ name, variable ] : *scope) {
		cout << variable->describe(depth, name + "[" + Names::Types[variable->type] + "]: ") << endl;
	}
}

Runtime::Runtime() { Bindings::Bind(this); init(global); }
Runtime::Runtime(string path, Reference in) { Bindings::Bind(this); result = load(path, in, global); }

Reference Runtime::load(string path, Reference in, Scope scope) {
	path = Search::Path(path);

	if (!scope) scope = global;
	init(scope, path, in);

	vector<Test> originalTests = Tests;
	Tests = {};

	ifstream stream = ifstream(path);
	
	if (!stream.good()) return Value::Empty();
	string bismuth((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

	if (Bismuth::Verbose) {
		cout << "\n\n--- Bismuth[" << path << "] ---" << endl;
		cout << endl << bismuth << endl;
	}

	Reference result = interpret(bismuth);
	scopes.pop();

	if (Bismuth::Testing && Tests.size() > 0) {
		cout << "\033[37m[ " << path << " ]\033[0m" << endl;
		cout << Utils::Boxed(Bismuth::Header, 68, 1, true, false) << endl << "\033[37m┊\033[0m" << endl;

		bool passed = true;
		int total = 0;
		for (int i = 0; i < Tests.size(); i++) {
			Test test = Tests[i];

			if (test.passed) {
				total++;
			} else {
				passed = false;
			}

			cout << ( test.passed ? "\033[92m" : "\033[91m" );
			cout << ( i == 0 ? "┎" : i == Tests.size() - 1 ? "┖" : "┠" ) << " " << ( test.passed ? "🟢" : "🔴" ) << " " << test.description << endl;
			cout << "\033[0m";
		}

		cout << ( passed ? "\033[92m" : "\033[91m" );
		cout << "┊" << endl << "└ " << ( passed ? "🟢" : "🔴" ) << " [\033[1m" << ( passed ? "SUCCESS" : "FAILURE" ) << "\033[0m" << ( passed ? "\033[92m" : "\033[91m" ) << "] " << total << " of " << Tests.size() << " passed" << endl << endl;
		cout << "\033[0m";

		result = Value::Make(passed);
	}

	Tests = originalTests;
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

	if (Bismuth::Verbose) {
		cout << "\n\n--- Tokens ---\n" << endl;
		Token::Log(&tokens);
	}

	return interpret(&tokens);
}

Reference Runtime::interpret(vector<Token>* tokens) {
	vector<Node> tree;
	Parse(tokens, &tree);

	if (Bismuth::Verbose) {
		cout << "\n\n--- Tree ---\n" << endl;
		Node::Log(&tree);
	}

	return interpret(&tree);
}

Reference Runtime::interpret(vector<Node>* tree) {
	vector<Annotated> instructions;
	Machine::Compile(this, tree, &instructions);

	if (Bismuth::Verbose) {
		cout << "\n\n--- Instructions ---\n" << endl;
		Machine::Log(this, &instructions);
		cout << endl;
	}

	Instance = this;
	Reference result = run(instructions);
	
	if (Bismuth::Verbose) {
		cout << "\n\n--- Result ---\n" << endl;
		cout << result->describe() << endl << endl;
		cout << "\n--- Globals ---\n" << endl;
		Log(global);
		cout << endl << endl;
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
				if (!Bismuth::Testing) return Value::Make(false);

				string description = arguments[0]->as<String>();
				Reference result = call(arguments[1]);

				bool pass = false;

				if (arguments[2]->is(Type::Block)) {
					pass = call(arguments[2], { Value::Copy(result) })->isTrue();
				} else {
					pass = result->equals(arguments[2]);
				}

				Tests.push_back({ description, pass });
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