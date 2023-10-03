void Analyse(string bismuth, vector<Token>* target, bool listing=false);
enum class Step { Next, Start, Stop, Stay, Over, Nest };

namespace Scanning {
	bool DotIndex = false;
	bool DotChain = false;
	bool Decimal = false;
	bool Gap = false;

	function<Step(Token*, Token*, vector<Token>*)> Subexpression = [](Token* token, Token* last, vector<Token>* tokens) {
		token->contents = token->contents.substr(1, token->contents.size() - 2);
		
		vector<Token> list;
		Analyse(token->contents, &list, true);

		token->children.push_back(list);
		return Step::Next;
	};
}

struct Scanner {
	function<Step(char, Token*)> cue;
	function<Step(char, Token*)> end;
	function<Step(Token*, Token*, vector<Token>*)> parse;
};

map<Mark, Scanner> Scanners = {
	{
		Mark::End,
		Scanner {
			[](char character, Token* last) {
				if (!last || last->is(vector<Mark>{ Mark::Comma, Mark::End })) return Step::Next;

				if (character == '\n') {
					if (last->is(Mark::Operator) && Operators[last->contents].pairing != Pairing::Unary) {
						return Step::Stop;
					} else {
						return Step::Stop;
					}
				}

				if (character == ';') return Step::Stop;
				return Step::Next;
			}
		}
	},
	{
		Mark::Comma,
		Scanner { [](char character, Token* last) { return character == ',' ? Step::Stop : Step::Next; } }
	},
	{
		Mark::Brackets,
		Scanner {
			[](char character, Token* last) { return (!last || last->is(vector<Mark>{ Mark::Operator, Mark::End })) && character == '(' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '(' ? Step::Nest : character == ')' ? Step::Stop : Step::Next; },
			Scanning::Subexpression
		}
	},
	{
		Mark::String,
		Scanner {
			[](char character, Token* last) { return character == '\'' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '\'' ? Step::Stop : Step::Next; },
			[](Token* token, Token* last, vector<Token>* tokens) { token->contents = token->contents.substr(1, token->contents.size() - 2); return Step::Next; }
		}
	},
	{
		Mark::Number,
		Scanner {
			[](char character, Token* last) { return isdigit(character) ? Step::Start : Step::Next; },
			[](char character, Token* last) {
				if (character == '.' && !Scanning::Decimal) { Scanning::Decimal = true; return Step::Next; }
				return isdigit(character) ? Step::Next : Step::Stay;
			},
			[](Token* token, Token* last, vector<Token>* tokens) {
				Token* prior = !tokens || tokens->size() < 2 ? nullptr : &(*tokens)[tokens->size() - 2];
				Scanning::Decimal = false;

				if (last && last->is(Mark::Operator, "-")) {
					if (!prior || prior->is(vector<Mark>{ Mark::Operator, Mark::End, Mark::Comma })) {
						token->contents = "-" + token->contents;
						return Step::Over;
					}
				}

				return Step::Next;
			}
		}
	},
	{
		Mark::Word,
		Scanner {
			[](char character, Token* last) { return isalpha(character) || character == '@' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return isalpha(character) || isdigit(character) ? Step::Next : Step::Stay; }
		}
	},
	{
		Mark::Operator,
		Scanner {
			[](char character, Token* last) {
				string op = { character };
				if (!Scanning::Gap && last && last->is(Mark::Operator) && Operators.count(last->contents + character)) op = last->contents + character;
				if (Operators.count(op)) return Step::Stop;
				for (const auto& [ symbol, _ ] : Operators) if (symbol.size() >= 2 && character == symbol.at(0)) return Step::Start;
				return Step::Next;
			},
			[](char character, Token* last) { return Step::Stay; },
			[](Token* token, Token* last, vector<Token>* tokens) {
				if (!Scanning::Gap && last && last->is(Mark::Operator) && Operators.count(last->contents + token->contents)) {
					token->contents = last->contents + token->contents;
					return Step::Over;
				}
				return Step::Next;
			}
		}
	},
	{
		Mark::Array,
		Scanner {
			[](char character, Token* last) { return (!last || last->is(vector<Mark>{ Mark::Operator, Mark::Comma, Mark::End })) && character == '[' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '[' ? Step::Nest : character == ']' ? Step::Stop : Step::Next; },
			Scanning::Subexpression
		}
	},
	{
		Mark::Index,
		Scanner {
			[](char character, Token* last) {
				Scanning::DotIndex = false;
				if (character == '.') { Scanning::DotIndex = true; return Step::Start; }
				return character == '[' ? Step::Start : Step::Next;
			},
			[](char character, Token* last) {
				Scanning::DotChain = false;
				if (Scanning::DotIndex) {
					return !isalpha(character) && !isdigit(character) ? Step::Stay : Step::Next;
				} else {
					return character == '[' ? Step::Nest : character == ']' ? Step::Stop : Step::Next;
				}
			},
			[](Token* token, Token* last, vector<Token>* tokens) {
				if (last && !last->is(Mark::Index)) token->children.push_back({ *last });
				if (last && last->is(Mark::Index)) token->children = last->children;

				vector<Token> list;
				string contents = token->contents.substr(1, token->contents.size() - 1);
				Analyse(Scanning::DotIndex ? "'" + contents + "'" : contents, &list, true);

				token->children.push_back(list);
				token->contents = ( last ? ( last->is(Mark::Array) ? "[ " + last->contents + " ]" : last->contents ) : "" ) + token->contents;

				return Step::Over;
			}
		}
	},
	{
		Mark::Block,
		Scanner {
			[](char character, Token* last) { return last && last->is(vector<Mark>{ Mark::Index, Mark::Array }) && character == '{' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '{' ? Step::Nest : character == '}' ? Step::Stop : Step::Next; },
			[](Token* token, Token* last, vector<Token>* tokens) {
				vector<Token> arguments;

				Analyse(last->contents, &arguments, true);
				token->children.push_back(arguments);

				vector<Token> body;

				Analyse(token->contents.substr(1, token->contents.size() - 2), &body);
				token->children.push_back(body);

				token->contents = "[" + last->contents + "]" + token->contents;
				return Step::Over;
			}
		}
	},
	{
		Mark::Call,
		Scanner {
			[](char character, Token* last) { return last && last->isnt(vector<Mark>{ Mark::Operator }) && character == '(' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '(' ? Step::Nest : character == ')' ? Step::Stop : Step::Next; },
			[](Token* token, Token* last, vector<Token>* tokens) {
				if (last && !last->is(Mark::Call)) token->children.push_back({ *last });
				if (last && last->is(Mark::Call)) token->children = last->children;

				vector<Token> list;
				string contents = token->contents.substr(1, token->contents.size() - 2);
				Analyse(contents, &list, true);

				token->children.push_back(list);
				token->contents = (last ? last->contents : "") + token->contents;

				return Step::Over;
			}
		}
	},
	{
		Mark::If,
		Scanner {
			[](char character, Token* last) { return last && last->is(Mark::Word, "if") ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '{' ? Step::Stay : Step::Next; },
			[](Token* token, Token* last, vector<Token>* tokens) {
				token->contents = token->contents.substr(1, token->contents.size() - 2);
				
				vector<Token> list;
				Analyse(token->contents, &list, true);

				token->children.push_back(list);
				return Step::Over;
			}
		}
	},
	{
		Mark::Else,
		Scanner {
			[](char character, Token* last) { return last && last->is(Mark::Word, "else") ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '{' ? Step::Stay : Step::Next; },
			[](Token* token, Token* last, vector<Token>* tokens) {
				Token* prior = &(*tokens)[tokens->size() - 2];
				
				token->mark = prior->mark;
				token->children = prior->children;

				vector<Token> list;
				Analyse(token->contents, &list, true);
				if (list.size() == 0) list.push_back(Token(Mark::Word, "true"));
				token->children.push_back(list);

				token->contents = prior->contents + " else " + token->contents;
				tokens->pop_back();

				return Step::Over;
			}
		}
	},
	{
		Mark::Do,
		Scanner {
			[](char character, Token* last) { return last && last->is(vector<Mark>{ Mark::If, Mark::Else }) && character == '{' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '{' ? Step::Nest : character == '}' ? Step::Stop : Step::Next; },
			[](Token* token, Token* last, vector<Token>* tokens) {
				token->mark = last->mark;
				token->children = last->children;
		
				vector<Token> list;
				Analyse(token->contents.substr(1, token->contents.size() - 2), &list, true);
				token->children.push_back(list);

				token->contents = last->contents + " " + token->contents;
				return Step::Over;
			}
		}
	},
	{
		Mark::When,
		Scanner {
			[](char character, Token* last) { return last && last->is(Mark::Word, "when") ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '{' ? Step::Stay : Step::Next; },
			[](Token* token, Token* last, vector<Token>* tokens) { return Step::Over; }
		}
	},
	{
		Mark::Translate,
		Scanner {
			[](char character, Token* last) { return last && last->is(Mark::When) && character == '{' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '{' ? Step::Nest : character == '}' ? Step::Stop : Step::Next; },
			Scanning::Subexpression
		}
	},
	{
		Mark::Table,
		Scanner {
			[](char character, Token* last) { return character == '{' ? Step::Start : Step::Next; },
			[](char character, Token* last) { return character == '{' ? Step::Nest : character == '}' ? Step::Stop : Step::Next; },
			Scanning::Subexpression
		}
	}
};