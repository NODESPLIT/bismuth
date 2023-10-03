enum class Task {
	Instruct,
	Literal,
	Array,
	Table,
	Block,
	Read,
	Inside,
	Define,
	Operate,
	Decide,
	Match,
	Call,
	Return,
	End
};

namespace Names {
	unordered_map<Task, string> Task = {
		{ Task::Instruct, "Instruct" },
		{ Task::Literal, "Literal" },
		{ Task::Array, "Array" },
		{ Task::Table, "Table" },
		{ Task::Block, "Block" },
		{ Task::Read, "Read" },
		{ Task::Inside, "Inside" },
		{ Task::Define, "Define" },
		{ Task::Operate, "Operate" },
		{ Task::Decide, "Decide" },
		{ Task::Match, "Match" },
		{ Task::Call, "Call" },
		{ Task::Return, "Return" },
		{ Task::End, "End" }
	};
};