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

	ifstream stream = ifstream(path);
	
	if (!stream.good()) return Value::Empty();
	string bismuth((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

	if (Bismuth::Verbose) {
		cout << "\n\n--- Bismuth[" << path << "] ---" << endl;
		cout << endl << bismuth << endl;
	}

	Reference result = interpret(bismuth);

	scopes.pop();
	return result;
}

Reference Runtime::import(string path, Reference in) {
	if (path.ends_with(".crystal")) {
		
		#if BOOST_VERSION >= 108400
		Crystals.push_back(boost::dll::import_symbol<Bismuth::API>(path, "crystal"));
		#else
		Crystals.push_back(boost::dll::import_alias<Bismuth::API>(path, "crystal"));
		#endif

		return Crystals.back()->construct(Instance);
	}

	Scope subscope = branched();
	std::filesystem::path base((*scopes.top())[Symbol::Context]->get("path")->as<string>()); base.remove_filename();

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
	vector<Instruction> instructions;
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