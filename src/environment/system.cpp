namespace System {
	void Bind(Scope environment, Parser* parser) {
		(*environment)["import"] = Value::Bound([parser](ArrayValue arguments) {
			string path = Search::Path(parser->subpath(arguments[0]->as<string>(), parser->current));
			string script = Load(path);

			vector<Lexer::Token> tokens;
			Lexer::Tokens(script, &tokens);

			Scope scope = make_shared<ScopeValue>();
			(*scope)["PATH"] = Value::Make(path);

			Reference result = parser->run(&tokens, scope);
			return result;
		});
	}
}