struct Argument {
	string name = "";
	vector<Lexer::Token> initial = {};
};

class Block {
	public:
		Scope context;
		
		vector<Lexer::Token> body = {};
		vector<Argument> arguments = {};
		unordered_map<string, Reference> cache = {};
		function<Reference(ArrayValue)> binding = nullptr;

		Block(function<Reference(ArrayValue)> binding) { this->binding = binding; }
		Block(Lexer::Token* token, Scope scope) {
			body = token->inside[1];
			context = scope;

			Argument argument = Argument();
			bool defaulting = false;

			for (int i = 0; i <= token->inside[0].size(); i++) {
				if (argument.name.empty()) {
					if (token->inside[0][i].is(Lexer::Token::Type::Word)) {
						argument.name = token->inside[0][i].contents;
					}
				} else {
					if (token->inside[0][i].is(Lexer::Token::Type::Comma) || i == token->inside[0].size()) {
						arguments.push_back(argument);
						argument = Argument();
						defaulting = false;
					} else if (token->inside[0][i].is(Lexer::Token::Type::Operator, "=")) {
						defaulting = true;
					} else {
						argument.initial.push_back(token->inside[0][i]);
					}
				}
			}
		}
};