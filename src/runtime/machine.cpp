namespace Machine {
	Reference VALUE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		return Instance->literals[instruction->mode];
	}

	Reference ARRAY(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		vector<int>* relations = &Instance->relations[instruction->children[0]];
		for (int i = 0; i < relations->size(); i++) Instance->literals[instruction->mode]->set(i, state[ (*relations)[i] ]);
		return Instance->literals[instruction->mode];
	}

	Reference TABLE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		vector<int>* relations = &Instance->relations[instruction->children[0]];
		for (int i = 0; i < relations->size(); i += 2) Instance->literals[instruction->mode]->set(state[ (*relations)[i] ]->as<String>(), state[ (*relations)[i + 1] ]);
		return Instance->literals[instruction->mode];
	}

	Reference BLOCK(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		Reference value = Instance->literals[instruction->mode];
		value->block->context = Instance->scopes.top();
		return value;
	}

	Reference READ(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		string word = Instance->words[instruction->mode];

		if (auto scope = Instance->scopes.top()->find(word); scope != Instance->scopes.top()->end()) {
			return scope->second;
		} else if (auto environment = Instance->environment->find(word); environment != Instance->environment->end() ) {
	    return environment->second;
		} else {
		  (*Instance->scopes.top())[word] = Value::Empty();
		  return (*Instance->scopes.top())[word];
		}
	}

	Reference INSIDE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		return state[instruction->children[0]]->get(state[instruction->children[1]]);
	}

	Reference DEFINE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		Reference left = state[instruction->children[0]];
		left->set(instruction->children[1] > -1 ? state[instruction->children[1]] : last);
		return left;
	}

	Reference OPERATE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		// cout << "OPERATE: " << instruction->children[0] << ": " << state[instruction->children[0]] << ", " << instruction->children[1] << ": " << state[instruction->children[1]] << endl;
		Reference left = state[instruction->children[0]];
		Reference right = instruction->children[1] == -1 ? last : state[instruction->children[1]];
		return Operate::List[ instruction->mode + ( ( int(left->type) * Operate::Count ) + int(right ? right->type : Type::Void) ) ](left, right);
	}

	Reference DECIDE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		if (last->isFalse()) {
			(*cursor).jump = instruction->mode;
			return Value::Empty();
		}
		return last;
	}

	Reference CALL(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		return Instance->call(state[instruction->children[0]], state[instruction->children[1]]->as<Array&>(), true);
	}

	Reference RETURN(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		Instance->returned = true;
		return last;
	}
}

namespace Machine {
	unordered_map<Task, Operation> Wiring = {
		{ Task::Value, &Machine::VALUE },
		{ Task::Array, &Machine::ARRAY },
		{ Task::Table, &Machine::TABLE },
		{ Task::Block, &Machine::BLOCK },
		{ Task::Read, &Machine::READ },
		{ Task::Inside, &Machine::INSIDE },
		{ Task::Define, &Machine::DEFINE },
		{ Task::Operate, &Machine::OPERATE },
		{ Task::Decide, &Machine::DECIDE },
		{ Task::Call, &Machine::CALL },
		{ Task::Return, &Machine::RETURN },
		{ Task::Jump, nullptr }
	};

	unordered_map<Operation, Task> Tasks = {
		{ &Machine::VALUE, Task::Value },
		{ &Machine::ARRAY, Task::Array },
		{ &Machine::TABLE, Task::Table },
		{ &Machine::BLOCK, Task::Block },
		{ &Machine::READ, Task::Read },
		{ &Machine::INSIDE, Task::Inside },
		{ &Machine::DEFINE, Task::Define },
		{ &Machine::OPERATE, Task::Operate },
		{ &Machine::DECIDE, Task::Decide },
		{ &Machine::CALL, Task::Call },
		{ &Machine::RETURN, Task::Return },
		{ nullptr, Task::Jump }
	};
}

namespace Names {
	unordered_map<Operation, string> Wiring = {
		{ &Machine::VALUE, "VALUE" },
		{ &Machine::ARRAY, "ARRAY" },
		{ &Machine::TABLE, "TABLE" },
		{ &Machine::BLOCK, "BLOCK" },
		{ &Machine::READ, "READ" },
		{ &Machine::INSIDE, "INSIDE" },
		{ &Machine::DEFINE, "DEFINE" },
		{ &Machine::OPERATE, "OPERATE" },
		{ &Machine::DECIDE, "DECIDE" },
		{ &Machine::CALL, "CALL" },
		{ &Machine::RETURN, "RETURN" },
		{ nullptr, "JUMP" }
	};
}