enum class Type {
	Any,
	Void,
	Boolean,
	Number,
	Range,
	String,
	Array,
	Table,
	Block,
};

namespace Names {
	unordered_map<Type, string> Types = {
		{ Type::Any, "Any" },
		{ Type::Void, "Void" },
		{ Type::Boolean, "Boolean" },
		{ Type::Number, "Number" },
		{ Type::Range, "Range" },
		{ Type::String, "String" },
		{ Type::Array, "Array" },
		{ Type::Table, "Table" },
		{ Type::Block, "Block" }
	};
}