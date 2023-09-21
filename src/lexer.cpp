namespace Lexer {
	enum Direction { Next, Start, Stop, Stay, Over, Nest };

	bool DotIndex = false;
	bool DotChain = false;
	bool Decimal = false;
	bool Gap = false;

	class Token {
		public:
			struct Type {
				string name;
				function<Direction(char, Token*)> cue;
				function<Direction(char, Token*)> end;
				function<Direction(Token*, Token*)> parse;
			};

			static const vector<Type> Types;

			static bool is(Token* token, string type) { return !token ? false : token->type < Types.size() && type.compare(Types[token->type].name) == 0; };
			static bool is(Token* token, vector<string> types) { if (!token) return false; for (auto &type : types) if (is(token, type)) return true; return false; };
			static bool isnt(Token* token, vector<string> types) { if (!token) return false; for (auto &type : types) if (is(token, type)) return false; return true; };

			static bool is(Token* token, string type, string value) { if (!token) return false; return is(token, type) && token->contains(value); };
			static bool is(Token* token, string type, vector<string> values) { if (!token) return false; return is(token, type) && token->contains(values); };

			static void Log(vector<Token>* tokens, int depth = 0) {
				for (auto &token : *tokens) {
					cout << Log::Indent(depth);

					regex flatten(R"(\s+)");
					cout << "[" << Token::Types[token.type].name << "] |" << regex_replace(token.contents, flatten, " ") << "|" << endl;

					if (Token::is(&token, "end")) cout << endl;
					
					for (int s = 0; s < token.inside.size(); s++){
						if (s != 0) { cout << Log::Indent(depth + 1) << "--" << endl; }
						Log(&token.inside[s], depth + 1);
					}
				}
			}

			bool contains(string value) { return contents.compare(value) == 0; }
			bool contains(vector<string> values) { for (auto &value : values) if (contains(value)) return true; return false; }

			bool is(string type) { return Token::is(this, type); }
			bool is(vector<string> types) { return Token::is(this, types); }
			bool isnt(vector<string> types) { return Token::isnt(this, types); }

			bool is(string type, string value) { return Token::is(this, type, value); }
			bool is(string type, vector<string> values) { return Token::is(this, type, values); }

			string describe() { return "[" + Types[type].name + "]: " + contents;  };

			int type;
			string contents;
			vector<vector<Token>> inside;

			Token(int type, string contents) {
				this->type = type;
				this->contents = contents;
			}
	};

	void Tokens(string script, vector<Token>* target, bool listing=false);

	function<Direction(Token*, Token*)> Subexpression = [](Token* token, Token* last) {
		token->contents = token->contents.substr(1, token->contents.size() - 2);
		
		vector<Token> list;
		Tokens(token->contents, &list, true);

		token->inside.push_back(list);
		return Direction::Next;
	};

	const vector<Token::Type> Token::Types = {
		Token::Type {
			"comma",
			[](char character, Token* last) { return character == ',' ? Direction::Stop : Direction::Next; }
		},
		Token::Type {
			"end",
			[](char character, Token* last) {
				if (!last || Token::is(last, vector<string>{ "comma", "end" })) return Direction::Next;
				if (character == '\n' && Token::isnt(last, vector<string>{ "operator" })) return Direction::Stop;
				if (character == ';') return Direction::Stop;
				return Direction::Next;
			}
		},
		Token::Type {
			"brackets",
			[](char character, Token* last) { return (!last || Token::is(last, vector<string>{ "operator" })) && character == '(' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '(' ? Direction::Nest : character == ')' ? Direction::Stop : Direction::Next; },
			Subexpression
		},
		Token::Type {
			"string",
			[](char character, Token* last) { return character == '\'' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '\'' ? Direction::Stop : Direction::Next; },
			[](Token* token, Token* last) { token->contents = token->contents.substr(1, token->contents.size() - 2); return Direction::Next; },
		},
		Token::Type {
			"number",
			[](char character, Token* last) { return isdigit(character) ? Direction::Start : Direction::Next; },
			[](char character, Token* last) {
				if (character == '.' && !Decimal) { Decimal = true; return Direction::Next; }
				return isdigit(character) ? Direction::Next : Direction::Stay;
			},
			[](Token* token, Token* last) { Decimal = false; return Direction::Next; }
		},
		Token::Type {
			"word",
			[](char character, Token* last) { return isalpha(character) ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return isalpha(character) || isdigit(character) ? Direction::Next : Direction::Stay; }
		},
		Token::Type {
			"range",
			[](char character, Token* last) { return (!last || Token::is(last, vector<string>{ "operator" })) && character == '<' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '>' ? Direction::Stop : Direction::Next; },
			Subexpression
		},
		Token::Type {
			"operator",
			[](char character, Token* last) {
				string op = { character };
				return Operator::List.count(op) ? Direction::Stop : Direction::Next;
			}
		},
		Token::Type {
			"array",
			[](char character, Token* last) { return (!last || Token::is(last, vector<string>{ "operator", "comma", "end" })) && character == '[' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '[' ? Direction::Nest : character == ']' ? Direction::Stop : Direction::Next; },
			Subexpression
		},
		Token::Type {
			"index",
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
			[](Token* token, Token* last) {
				if (last && !last->is("index")) token->inside.push_back({ *last });
				if (last && last->is("index")) token->inside = last->inside;

				vector<Token> list;
				string contents = token->contents.substr(1, token->contents.size() - 1);
				Tokens(DotIndex ? "'" + contents + "'" : contents, &list, true);

				token->inside.push_back(list);
				token->contents = ( last ? ( last->is("array") ? "[ " + last->contents + " ]" : last->contents ) : "" ) + token->contents;

				return Direction::Over;
			}
		},
		Token::Type {
			"block",
			[](char character, Token* last) { return Token::is(last, vector<string>{ "index", "array" }) && character == '{' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
			[](Token* token, Token* last) {
				vector<Token> arguments;

				Tokens(last->contents, &arguments, true);
				token->inside.push_back(arguments);

				vector<Token> body;

				Tokens(token->contents.substr(1, token->contents.size() - 2), &body);
				token->inside.push_back(body);

				token->contents = "[" + last->contents + "]" + token->contents;
				return Direction::Over;
			}
		},
		Token::Type {
			"call",
			[](char character, Token* last) { return last && Token::isnt(last, vector<string>{ "operator" }) && character == '(' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '(' ? Direction::Nest : character == ')' ? Direction::Stop : Direction::Next; },
			[](Token* token, Token* last) {
				if (last && !last->is("call")) token->inside.push_back({ *last });
				if (last && last->is("call")) token->inside = last->inside;

				vector<Token> list;
				string contents = token->contents.substr(1, token->contents.size() - 2);
				Tokens(contents, &list, true);

				token->inside.push_back(list);
				token->contents = (last ? last->contents : "") + token->contents;

				return Direction::Over;
			}
		},
		Token::Type {
			"if",
			[](char character, Token* last) { return Token::is(last, "word", "if") ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '{' ? Direction::Stay : Direction::Next; },
			[](Token* token, Token* last) { return Direction::Over; }
		},
		Token::Type {
			"else",
			[](char character, Token* last) { return Token::is(last, "word", "else") ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '{' ? Direction::Stay : Direction::Next; },
			[](Token* token, Token* last) { return Direction::Over; }
		},
		Token::Type {
			"do",
			[](char character, Token* last) { return Token::is(last, vector<string>{ "if", "else" }) && character == '{' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
			Subexpression
		},
		Token::Type {
			"when",
			[](char character, Token* last) { return Token::is(last, "word", "when") ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '{' ? Direction::Stay : Direction::Next; },
			[](Token* token, Token* last) { return Direction::Over; }
		},
		Token::Type {
			"translate",
			[](char character, Token* last) { return Token::is(last, "when") && character == '{' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
			Subexpression
		},
		Token::Type {
			"table",
			[](char character, Token* last) { return character == '{' ? Direction::Start : Direction::Next; },
			[](char character, Token* last) { return character == '{' ? Direction::Nest : character == '}' ? Direction::Stop : Direction::Next; },
			Subexpression
		}
	};

	void Tokens(string script, vector<Token>* target, bool listing) {
		Token* last = nullptr;
		Direction direction;

		int cued = -1;
		int nesting = 0;

		string contents = "";
		bool complete = false;
		bool stay = false;
		bool single = false;

		int position = 0;
		while (position <= script.size()) {
			char character = position < script.size() ? script[position] : ' ';

			if (complete) {
				Token token = Token(cued, stay ? contents.substr(0, contents.size() - 1) : contents);
				
				if (Token::Types[cued].parse && Token::Types[cued].parse(&token, last) == Direction::Over && target->size() > 0) {
					(*target)[target->size() - 1] = token;
				} else {
					target->push_back(token);
				}
				
				if (!single && !stay) position++;

				last = &(*target)[target->size() - 1];

				cued = -1;
				nesting = 0;

				contents = "";
				complete = false;
				stay = false;
				single = false;

				continue;
			}

			if (cued > -1) {
				contents += character;

				direction = Token::Types[cued].end(character, last);
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
				for (int i = 0; i < Token::Types.size(); i++) {
					direction = Token::Types[i].cue(character, last);
					
					if (direction == Direction::Next) continue;
					if (direction == Direction::Stop) { complete = true; single = true; }

					cued = i;
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