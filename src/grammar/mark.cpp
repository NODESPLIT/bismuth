enum class Mark {
	None,
	End,
	Comma,
	Brackets,
	String,
	Based,
	Number,
	Word,
	Operator,
	Unary,
	Array,
	Index,
	Block,
	Call,
	If,
	Else,
	Do,
	When,
	Translate,
	Table
};

namespace Names {
	unordered_map<Mark, string> Mark = {
		{ Mark::End, "End" },
		{ Mark::Comma, "Comma" },
		{ Mark::Brackets, "Brackets" },
		{ Mark::String, "String" },
		{ Mark::Based, "Based" },
		{ Mark::Number, "Number" },
		{ Mark::Word, "Word" },
		{ Mark::Operator, "Operator" },
		{ Mark::Unary, "Unary" },
		{ Mark::Array, "Array" },
		{ Mark::Index, "Index" },
		{ Mark::Block, "Block" },
		{ Mark::Call, "Call" },
		{ Mark::If, "If" },
		{ Mark::Else, "Else" },
		{ Mark::Do, "Do" },
		{ Mark::When, "When" },
		{ Mark::Translate, "Translate" },
		{ Mark::Table, "Table" }
	};
}