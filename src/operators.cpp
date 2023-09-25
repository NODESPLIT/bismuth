typedef function<Reference(Reference, Reference, Parser* parser, Scope scope)> Operation;

enum Type { Boolean, Number, Range, String, Array, Table, Block, Void, Any };
unordered_map<Type, string> Typename = {
	{ Boolean, "Boolean" },
	{ Number, "Number" },
	{ Range, "Range" },
	{ String, "String" },
	{ Table, "Table" },
	{ Array, "Array" },
	{ Block, "Block" },
	{ Void, "Void" }
};

struct Operator {
	public:
		static unordered_map<string, Operator> List;
		enum Pairing { Unary, Binary, Ternary };
		enum Category { Unery, Arithmetic, Relational, Logical, Bitwise, Assignment, Conditional, Inside };
		int priority;
		Pairing pairing;
		Category category;
		unordered_map<Type, unordered_map<Type, Operation>> operations;
};

// const map<string, Operator> Operators = {
// 	{ "++", Operator{ Operator::Pairing::Unary, Operator::Category::Unery } },
// 	{ "--", Operator{ Operator::Pairing::Unary, Operator::Category::Unery } },
// 	{ "+", Operator{ Operator::Pairing::Binary, Operator::Category::Arithmetic } },
// 	{ "-", Operator{ Operator::Pairing::Binary, Operator::Category::Arithmetic } },
// 	{ "*", Operator{ Operator::Pairing::Binary, Operator::Category::Arithmetic } },
// 	{ "/", Operator{ Operator::Pairing::Binary, Operator::Category::Arithmetic } },
// 	{ "%", Operator{ Operator::Pairing::Binary, Operator::Category::Arithmetic } },
// 	{ "<", Operator{ Operator::Pairing::Binary, Operator::Category::Relational } },
// 	{ "<=", Operator{ Operator::Pairing::Binary, Operator::Category::Relational } },
// 	{ ">", Operator{ Operator::Pairing::Binary, Operator::Category::Relational } },
// 	{ ">=", Operator{ Operator::Pairing::Binary, Operator::Category::Relational } },
// 	{ "==", Operator{ Operator::Pairing::Binary, Operator::Category::Relational } },
// 	{ "!=", Operator{ Operator::Pairing::Binary, Operator::Category::Relational } },
// 	{ "&&", Operator{ Operator::Pairing::Binary, Operator::Category::Logical } },
// 	{ "||", Operator{ Operator::Pairing::Binary, Operator::Category::Logical } },
// 	{ "!", Operator{ Operator::Pairing::Binary, Operator::Category::Logical } },
// 	{ "&", Operator{ Operator::Pairing::Binary, Operator::Category::Bitwise } },
// 	{ "|", Operator{ Operator::Pairing::Binary, Operator::Category::Bitwise } },
// 	{ "<<", Operator{ Operator::Pairing::Binary, Operator::Category::Bitwise } },
// 	{ ">>", Operator{ Operator::Pairing::Binary, Operator::Category::Bitwise } },
// 	{ "~", Operator{ Operator::Pairing::Binary, Operator::Category::Bitwise } },
// 	{ "^", Operator{ Operator::Pairing::Binary, Operator::Category::Bitwise } },
// 	{ "=", Operator{ Operator::Pairing::Binary, Operator::Category::Assignment } },
// 	{ "+=", Operator{ Operator::Pairing::Binary, Operator::Category::Assignment } },
// 	{ "-=", Operator{ Operator::Pairing::Binary, Operator::Category::Assignment } },
// 	{ "*=", Operator{ Operator::Pairing::Binary, Operator::Category::Assignment } },
// 	{ "/=", Operator{ Operator::Pairing::Binary, Operator::Category::Assignment } },
// 	{ "%=", Operator{ Operator::Pairing::Binary, Operator::Category::Assignment } },
// 	{ "?", Operator{ Operator::Pairing::Ternary, Operator::Category::Conditional } },
// 	{ ":", Operator{ Operator::Pairing::Ternary, Operator::Category::Conditional } }
// };