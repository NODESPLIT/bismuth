enum class Pairing { Unary, Binary, Ternary };
enum class Effect { Unary, Arithmetic, Relational, Logical, Bitwise, Assignment, Conditional, Inside };
enum class Orientation { LeftToRight, RightToLeft };

struct Operator {
	int priority;
	Pairing pairing;
	Effect effect;
	Orientation orientation;
	string suboperator = "";
};

unordered_map<string, Operator> Operators = {
	{ "~:", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ ":-", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ "-:", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ "+:", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ "%:", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ "::", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ ":", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ "=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft } },
	// { "...", Operator { 8, Pairing::Unary, Effect::Assignment, Orientation::RightToLeft } },
	{ ":=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, ":" } },
	{ "+=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "+" } },
	{ "-=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "-" } },
	{ "*=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "*" } },
	{ "/=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "/" } },
	{ "%=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "%" } },
	{ "++", Operator { 1, Pairing::Unary, Effect::Unary, Orientation::RightToLeft, "++" } },
	{ "--", Operator { 1, Pairing::Unary, Effect::Unary, Orientation::RightToLeft, "--" } },
	{ "->", Operator { 3, Pairing::Binary, Effect::Inside, Orientation::LeftToRight } },
	{ "+", Operator { 3, Pairing::Binary, Effect::Arithmetic, Orientation::LeftToRight } },
	{ "-", Operator { 3, Pairing::Binary, Effect::Arithmetic, Orientation::LeftToRight } },
	{ "*", Operator { 2, Pairing::Binary, Effect::Arithmetic, Orientation::LeftToRight } },
	{ "/", Operator { 2, Pairing::Binary, Effect::Arithmetic, Orientation::LeftToRight } },
	{ "%", Operator { 2, Pairing::Binary, Effect::Arithmetic, Orientation::LeftToRight } },
	{ "<", Operator { 4, Pairing::Binary, Effect::Relational, Orientation::LeftToRight } },
	{ "<=", Operator { 4, Pairing::Binary, Effect::Relational, Orientation::LeftToRight } },
	{ ">", Operator { 4, Pairing::Binary, Effect::Relational, Orientation::LeftToRight } },
	{ ">=", Operator { 4, Pairing::Binary, Effect::Relational, Orientation::LeftToRight } },
	{ "==", Operator { 5, Pairing::Binary, Effect::Relational, Orientation::LeftToRight } },
	{ "!=", Operator { 5, Pairing::Binary, Effect::Relational, Orientation::LeftToRight } },
	{ "&", Operator { 11, Pairing::Binary, Effect::Bitwise, Orientation::LeftToRight } },
	{ "|", Operator { 13, Pairing::Binary, Effect::Bitwise, Orientation::LeftToRight } },
	{ "^", Operator { 12, Pairing::Binary, Effect::Bitwise, Orientation::LeftToRight } },
	{ "~", Operator { 3, Pairing::Unary, Effect::Bitwise, Orientation::LeftToRight } },
	{ "!", Operator { 3, Pairing::Unary, Effect::Logical, Orientation::LeftToRight } },
	{ "&&", Operator { 11, Pairing::Binary, Effect::Logical, Orientation::LeftToRight } },
	{ "||", Operator { 11, Pairing::Binary, Effect::Logical, Orientation::LeftToRight } },
	{ "<<", Operator { 7, Pairing::Binary, Effect::Bitwise, Orientation::LeftToRight } },
	{ ">>", Operator { 7, Pairing::Binary, Effect::Bitwise, Orientation::LeftToRight } }
};