enum class Type {
	Void,
	Boolean,
	Number,
	Range,
	String,
	Array,
	Table,
	Block,
	Any
};

namespace Names {
	unordered_map<Type, string> Types = {
		{ Type::Void, "Void" },
		{ Type::Boolean, "Boolean" },
		{ Type::Number, "Number" },
		{ Type::Range, "Range" },
		{ Type::String, "String" },
		{ Type::Array, "Array" },
		{ Type::Table, "Table" },
		{ Type::Block, "Block" },
		{ Type::Any, "Any" }
	};
}