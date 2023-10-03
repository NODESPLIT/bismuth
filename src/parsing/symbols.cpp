unordered_map<string, Node> Symbols = {
	{ "void", Node{ Task::Literal, Mark::Word, "void" } },
	{ "true", Node{ Task::Literal, Mark::Word, "true" } },
	{ "false", Node{ Task::Literal, Mark::Word, "false" } }
};