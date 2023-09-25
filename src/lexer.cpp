namespace Lexer {
	enum Direction { Next, Start, Stop, Stay, Over, OverTwice, Nest };

	bool DotIndex = false;
	bool DotChain = false;
	bool Decimal = false;
	bool Gap = false;

	class Token {
		public:
			enum class Type { None, End, Comma, Brackets, String, Number, Word, Operator, Array, Index, Block, Call, If, Else, Do, When, Translate, Table };

			enum class Symbol { None, Return, True, False, Void };
			static unordered_map<string, Symbol> Symbols;

			struct Scan {
				Type type;
				function<Direction(char, Token*)> cue;
				function<Direction(char, Token*)> end;
				function<Direction(Token*, Token*, vector<Token>*)> parse;
			};

			static unordered_map<Type, string> Names;
			static map<Type, Scan> Scans;

			static bool is(Token* token, Type type) { return !token ? false : token->type == type; };
			static bool is(Token* token, vector<Type> types) { if (!token) return false; for (auto &type : types) if (is(token, type)) return true; return false; };
			static bool isnt(Token* token, vector<Type> types) { if (!token) return false; for (auto &type : types) if (is(token, type)) return false; return true; };

			static bool is(Token* token, Type type, string value) { if (!token) return false; return is(token, type) && token->contains(value); };
			static bool is(Token* token, Type type, vector<string> values) { if (!token) return false; return is(token, type) && token->contains(values); };

			static void Log(vector<Token>* tokens, int depth = 0) {
				for (auto &token : *tokens) {
					cout << Log::Indent(depth);

					regex flatten(R"(\s+)");
					cout << "[" << Names[token.type] << "] |" << regex_replace(token.contents, flatten, " ") << "|" << endl;

					if (Token::is(&token, Type::End)) cout << endl;
					
					for (int s = 0; s < token.inside.size(); s++){
						if (s != 0) { cout << Log::Indent(depth + 1) << "--" << endl; }
						Log(&token.inside[s], depth + 1);
					}
				}
			}

			bool contains(string value) { return contents.compare(value) == 0; }
			bool contains(vector<string> values) { for (auto &value : values) if (contains(value)) return true; return false; }

			bool is(Type type) { return Token::is(this, type); }
			bool is(vector<Type> types) { return Token::is(this, types); }
			bool isnt(vector<Type> types) { return Token::isnt(this, types); }

			bool is(Type type, string value) { return Token::is(this, type, value); }
			bool is(Type type, vector<string> values) { return Token::is(this, type, values); }

			string describe() { return "[" + Token::Names[type] + "]: " + contents;  };

			Type type = Type::End;
			Symbol symbol = Symbol::None;

			string contents = "";
			vector<vector<Token>> inside = {};

			Token(Type type, string contents) {
				this->type = type;
				this->contents = contents;
				if (type == Type::Word && Symbols.count(contents)) symbol = Symbols[contents];
			}
	};

	void Tokens(string script, vector<Token>* target, bool listing=false);

	function<Direction(Token*, Token*, vector<Token>*)> Subexpression = [](Token* token, Token* last, vector<Token>* tokens) {
		token->contents = token->contents.substr(1, token->contents.size() - 2);
		
		vector<Token> list = {};
		Tokens(token->contents, &list, true);

		token->inside.push_back(list);
		return Direction::Next;
	};

	unordered_map<Token::Type, string> Token::Names = {
		{ Token::Type::End, "End" },
		{ Token::Type::Comma, "Comma" },
		{ Token::Type::Brackets, "Brackets" },
		{ Token::Type::String, "String" },
		{ Token::Type::Number, "Number" },
		{ Token::Type::Word, "Word" },
		{ Token::Type::Operator, "Operator" },
		{ Token::Type::Array, "Array" },
		{ Token::Type::Index, "Index" },
		{ Token::Type::Block, "Block" },
		{ Token::Type::Call, "Call" },
		{ Token::Type::If, "If" },
		{ Token::Type::Else, "Else" },
		{ Token::Type::Do, "Do" },
		{ Token::Type::When, "When" },
		{ Token::Type::Translate, "Translate" },
		{ Token::Type::Table, "Table" }
	};

	unordered_map<string, Token::Symbol> Token::Symbols = {
		{ "", Token::Symbol::None },
		{ "return", Token::Symbol::Return },
		{ "true", Token::Symbol::True },
		{ "false", Token::Symbol::False },
		{ "void", Token::Symbol::Void }
	};

	map<Token::Type, Token::Scan> Token::Scans = {
		{
			Token::Type::End,
			Token::Scan {
				Token::Type::End,
				[](char character, Token* last) {
					if (!last || Token::is(last, vector<Token::Type>{ Token::Type::Comma, Token::Type::End })) return Direction::Next;

					if (character == '\n') {
						if (last->is(Token::Type::Operator) && Operator::List[last->contents].pairing != Operator::Pairing::Unary) {
							return Direction::Stop;
						} else {
							return Direction::Stop;
						}
					}

					if (character == ';') return Direction::Stop;
					return Direction::Next;
				}
			}
		},
		{
			Token::Type::Comma,
			Token::Scan {
				Token::Type::Comma,
				[](char character, Token* last) { return character == ',' ? Direction::Stop : Direction::Next; }
			}
		},
		{
			Token::Type::Brackets,
			Token::Scan {
				Token::Type::Brackets,
				[](char character, Token* last) { return (!last || Token::is(last, vector<Token::Type>{ Token::Type::Operator, Token::Type::End })) && character == '(' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '(' ? Direction::Nest : character == ')' ? Direction::Stop : Direction::Next; },
				Subexpression
			}
		},
		{
			Token::Type::String,
			Token::Scan {
				Token::Type::String,
				[](char character, Token* last) { return character == '\'' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '\'' ? Direction::Stop : Direction::Next; },
				[](Token* token, Token* last, vector<Token>* tokens) { token->contents = token->contents.substr(1, token->contents.size() - 2); return Direction::Next; }
			}
		},
		{
			Token::Type::Number,
			Token::Scan {
				Token::Type::Number,
				[](char character, Token* last) { return isdigit(character) ? Direction::Start : Direction::Next; },
				[](char character, Token* last) {
					if (character == '.' && !Decimal) { Decimal = true; return Direction::Next; }
					return isdigit(character) ? Direction::Next : Direction::Stay;
				},
				[](Token* token, Token* last, vector<Token>* tokens) {
					Token* prior = !tokens || tokens->size() < 2 ? nullptr : &(*tokens)[tokens->size() - 2];
					Decimal = false;

					if (last && last->is(Token::Type::Operator, "-")) {
						if (!prior || prior->is(vector<Token::Type>{ Token::Type::Operator, Token::Type::End, Token::Type::Comma })) {
							token->contents = "-" + token->contents;
							return Direction::Over;
						}
					}

					return Direction::Next;
				}
			}
		},
		{
			Token::Type::Word,
			Token::Scan {
				Token::Type::Word,
				[](char character, Token* last) { return isalpha(character) || character == '@' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return isalpha(character) || isdigit(character) ? Direction::Next : Direction::Stay; }
			}
		},
		{
			Token::Type::Operator,
			Token::Scan {
				Token::Type::Operator,
				[](char character, Token* last) {
					string op = { character };
					if (!Gap && last && Token::is(last, Token::Type::Operator) && Operator::List.count(last->contents + character)) op = last->contents + character;
					if (Operator::List.count(op)) return Direction::Stop;
					for (const auto& [ symbol, _ ] : Operator::List) if (symbol.size() >= 2 && character == symbol.at(0)) return Direction::Start;
					return Direction::Next;
				},
				[](char character, Token* last) { return Direction::Stay; },
				[](Token* token, Token* last, vector<Token>* tokens) {
					if (!Gap && last && Token::is(last, Token::Type::Operator) && Operator::List.count(last->contents + token->contents)) {
						token->contents = last->contents + token->contents;
						return Direction::Over;
					}
					return Direction::Next;
				}
			}
		},
		{
			Token::Type::Array,
			Token::Scan {
				Token::Type::Array,
				[](char character, Token* last) { return (!last || Token::is(last, vector<Token::Type>{ Token::Type::Operator, Token::Type::Comma, Token::Type::End })) && character == '[' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '[' ? Direction::Nest : character == ']' ? Direction::Stop : Direction::Next; },
				Subexpression
			}
		},
		{
			Token::Type::Index,
			Token::Scan {
				Token::Type::Index,
				[](char character, Token* last) {
					DotIndex = false;
					if (character == '.') { DotIndex = true; return Direction::Start; }
					return character == '[' ? Direction::Start : Direction::Next;
				},
				[](char character, Token* last) {
					DotChain = false;
					if (DotIndex) {
						return !isalpha(character) && !isdigit(character) ? Direction::Stay : Direction::Next;
					} else {
						return character == '[' ? Direction::Nest : character == ']' ? Direction::Stop : Direction::Next;
					}
				},
				[](Token* token, Token* last, vector<Token>* tokens) {
					if (last && !last->is(Token::Type::Index)) token->inside.push_back({ *last });
					if (last && last->is(Token::Type::Index)) token->inside = last->inside;

					vector<Token> list = {};
					string contents = token->contents.substr(1, token->contents.size() - 1);
					Tokens(DotIndex ? "'" + contents + "'" : contents, &list, true);

					token->inside.push_back(list);
					token->contents = ( last ? ( last->is(Token::Type::Array) ? "[ " + last->contents + " ]" : last->contents ) : "" ) + token->contents;

					return Direction::Over;
				}
			}
		},
		{
			Token::Type::Block,
			Token::Scan {
				Token::Type::Block,
				[](char character, Token* last) { return Token::is(last, vector<Token::Type>{ Token::Type::Index, Token::Type::Array }) && character == '{' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
				[](Token* token, Token* last, vector<Token>* tokens) {
					vector<Token> arguments = {};

					Tokens(last->contents, &arguments, true);
					token->inside.push_back(arguments);

					vector<Token> body = {};

					Tokens(token->contents.substr(1, token->contents.size() - 2), &body);
					token->inside.push_back(body);

					token->contents = "[" + last->contents + "]" + token->contents;
					return Direction::Over;
				}
			}
		},
		{
			Token::Type::Call,
			Token::Scan {
				Token::Type::Call,
				[](char character, Token* last) { return last && Token::isnt(last, vector<Token::Type>{ Token::Type::Operator }) && character == '(' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '(' ? Direction::Nest : character == ')' ? Direction::Stop : Direction::Next; },
				[](Token* token, Token* last, vector<Token>* tokens) {
					if (last && !last->is(Token::Type::Call)) token->inside.push_back({ *last });
					if (last && last->is(Token::Type::Call)) token->inside = last->inside;

					vector<Token> list = {};
					string contents = token->contents.substr(1, token->contents.size() - 2);
					Tokens(contents, &list, true);

					token->inside.push_back(list);
					token->contents = (last ? last->contents : "") + token->contents;

					return Direction::Over;
				}
			}
		},
		{
			Token::Type::If,
			Token::Scan {
				Token::Type::If,
				[](char character, Token* last) { return Token::is(last, Token::Type::Word, "if") ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '{' ? Direction::Stay : Direction::Next; },
				[](Token* token, Token* last, vector<Token>* tokens) {
					token->contents = token->contents.substr(1, token->contents.size() - 2);
					
					vector<Token> list = {};
					Tokens(token->contents, &list, true);

					token->inside.push_back(list);
					return Direction::Over;
				}
			}
		},
		{
			Token::Type::Else,
			Token::Scan {
				Token::Type::Else,
				[](char character, Token* last) { return Token::is(last, Token::Type::Word, "else") ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '{' ? Direction::Stay : Direction::Next; },
				[](Token* token, Token* last, vector<Token>* tokens) {
					Token* prior = &(*tokens)[tokens->size() - 2];
					
					token->type = prior->type;
					token->inside = prior->inside;

					vector<Token> list = {};
					Tokens(token->contents, &list, true);
					if (list.size() == 0) list.push_back(Token(Token::Type::Word, "true"));
					token->inside.push_back(list);

					token->contents = prior->contents + " else " + token->contents;
					tokens->pop_back();

					return Direction::Over;
				}
			}
		},
		{
			Token::Type::Do,
			Token::Scan {
				Token::Type::Do,
				[](char character, Token* last) { return Token::is(last, vector<Token::Type>{ Token::Type::If, Token::Type::Else }) && character == '{' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
				[](Token* token, Token* last, vector<Token>* tokens) {
					token->type = last->type;
					token->inside = last->inside;
			
					vector<Token> list = {};
					Tokens(token->contents.substr(1, token->contents.size() - 2), &list, true);
					token->inside.push_back(list);

					token->contents = last->contents + " " + token->contents;
					return Direction::Over;
				}
			}
		},
		{
			Token::Type::When,
			Token::Scan {
				Token::Type::When,
				[](char character, Token* last) { return Token::is(last, Token::Type::Word, "when") ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '{' ? Direction::Stay : Direction::Next; },
				[](Token* token, Token* last, vector<Token>* tokens) { return Direction::Over; }
			}
		},
		{
			Token::Type::Translate,
			Token::Scan {
				Token::Type::Translate,
				[](char character, Token* last) { return Token::is(last, Token::Type::When) && character == '{' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
				Subexpression
			}
		},
		{
			Token::Type::Table,
			Token::Scan {
				Token::Type::Table,
				[](char character, Token* last) { return character == '{' ? Direction::Start : Direction::Next; },
				[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
				Subexpression
			}
		}
	};

	void Tokens(string script, vector<Token>* target, bool listing) {
		Token* last = nullptr;
		Direction direction;

		Token::Type cued = Token::Type::None;
		int nesting = 0;

		string contents = "";
		bool complete = false;
		bool stay = false;
		bool single = false;

		char character;
		char before;
		int comment = 0;

		int position = 0;
		while (position <= script.size()) {
			before = character;
			character = position < script.size() ? script[position] : ' ';

			if (!Gap && !comment) comment = before == '/' && character == '/' ? 1 : before == '/' && character == '*' ? 2 : 0;
			if (comment == 2 && before == '*' && character == '/') { comment = false; position++; continue; }
			if (comment == 1 && character == '\n') comment = false;
			if (comment) { position++; continue; }

			if (complete) {
				Token token = Token(cued, stay ? contents.substr(0, contents.size() - 1) : contents);
				Direction parsed = Token::Scans[cued].parse ? Token::Scans[cued].parse(&token, last, target) : Direction::Next;

				if (parsed == Direction::Over && target->size() > 0) {
					(*target)[target->size() - 1] = token;
				} else {
					target->push_back(token);
				}
				
				if (!single && !stay) position++;

				last = &(*target)[target->size() - 1];

				cued = Token::Type::None;
				nesting = 0;

				contents = "";
				complete = false;
				stay = false;
				single = false;

				continue;
			}

			if (cued != Token::Type::None) {
				contents += character;

				direction = Token::Scans[cued].end(character, last);
				if (direction == Direction::Nest) nesting++;

				if (nesting == 0) {
					if (direction == Direction::Stay) stay = true;
					if (direction == Direction::Stop || direction == Direction::Stay) { complete = true; continue; }
				} else if (direction == Direction::Stop || direction == Direction::Stay) {
					nesting--;
				}

				position++;
				continue;
			}

			if (character == ',' && !listing) {
				position++;
				continue;
			}

			if (character != '	' || character != ' ') {
				for (const auto& [ type, scan ] : Token::Scans) {
					direction = scan.cue(character, last);
					
					if (direction == Direction::Next) continue;
					if (direction == Direction::Stop) { complete = true; single = true; }

					cued = type;
					contents += character;
					break;
				}

				Gap = false;
			} else {
				Gap = true;
			}

			position++;
		}
	};
};