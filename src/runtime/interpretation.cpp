static void Log(Scope scope, int depth=0) {
	for (const auto& [ name, variable ] : *scope) {
		cout << variable->describe(depth, name + "[" + Names::Types[variable->type] + "]: ") << endl;
	}
}

Reference load(string path, Reference in=nullptr, Scope scope=nullptr) {
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

Reference import(string path, Reference in=nullptr) {
	Scope subscope = branched();
	std::filesystem::path base((*scopes.top())[Symbol::Context]->get("path")->as<string>()); base.remove_filename();
	return load(base / path, in, subscope);
}

Reference interpret(string bismuth) {
	vector<Token> tokens;
	Analyse(bismuth, &tokens);

	if (Bismuth::Verbose) {
		cout << "\n\n--- Tokens ---\n" << endl;
		Token::Log(&tokens);
	}

	return interpret(&tokens);
}

Reference interpret(vector<Token>* tokens) {
	vector<Node> tree;
	Parse(tokens, &tree);

	if (Bismuth::Verbose) {
		cout << "\n\n--- Tree ---\n" << endl;
		Node::Log(&tree);
	}

	return interpret(&tree);
}

Reference interpret(vector<Node>* tree) {
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