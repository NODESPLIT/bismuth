enum class Pairing { Unary, Binary, Ternary };
enum class Effect { Unery, Arithmetic, Relational, Logical, Bitwise, Assignment, Conditional, Inside };
enum class Orientation { LeftToRight, RightToLeft };

struct Operator {
	int priority;
	Pairing pairing;
	Effect effect;
	Orientation orientation;
	string suboperator = "";
};

map<string, Operator> Operators = {
	{ ":", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::RightToLeft } },
	{ "->", Operator { 8, Pairing::Binary, Effect::Inside, Orientation::RightToLeft } },
	{ "=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft } },
	{ ":=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, ":" } },
	{ "+=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "+" } },
	{ "-=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "-" } },
	{ "*=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "*" } },
	{ "/=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "/" } },
	{ "%=", Operator { 8, Pairing::Binary, Effect::Assignment, Orientation::RightToLeft, "%" } },
	{ "++", Operator { 1, Pairing::Unary, Effect::Unery, Orientation::RightToLeft, "++" } },
	{ "--", Operator { 1, Pairing::Unary, Effect::Unery, Orientation::RightToLeft, "--" } },
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
	{ "!=", Operator { 5, Pairing::Binary, Effect::Relational, Orientation::LeftToRight } }
};