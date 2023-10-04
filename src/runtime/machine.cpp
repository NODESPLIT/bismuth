namespace Machine {
	Reference VALUE(Instruction* instruction) {
		return Instance->literals[instruction->index];
	}

	Reference ARRAY(Instruction* instruction) {
		for (int i = 0; i < instruction->children.size(); i++) Instance->literals[instruction->index]->set(i, instruction->children[i].resolve());
		return Instance->literals[instruction->index];
	}

	Reference TABLE(Instruction* instruction) {
		for (int i = 0; i < instruction->children.size(); i += 2) Instance->literals[instruction->index]->set(Instance->words[instruction->children[i].index], instruction->children[i + 1].resolve());
		return Instance->literals[instruction->index];
	}

	Reference BLOCK(Instruction* instruction) {
		Reference value = Instance->literals[instruction->index];
		value->block->context = Instance->current;
		return value;
	}

	Reference READ(Instruction* instruction) {
		string word = Instance->words[instruction->index];

		if (auto current = Instance->current->find(word); current != Instance->current->end()) {
			return current->second;
		} else if (auto environment = Instance->environment->find(word); environment != Instance->environment->end() ) {
	    return environment->second;
		} else {
		  return (*Instance->current)[word] = Value::Empty();
		}
	}

	Reference INSIDE(Instruction* instruction) {
		return instruction->children[0].resolve()->get(instruction->children[1].resolve());
	}

	Reference DEFINE(Instruction* instruction) {
		Reference left = instruction->children[0].resolve();
		left->set(instruction->children[1].resolve());
		return left;
	}

	Reference OPERATE(Instruction* instruction) {
		Reference left = instruction->children[0].resolve();
		Reference right = instruction->children[1].resolve();
		return Operate::List[ instruction->index + ( ( int(left->type) * Operate::Types ) + int(right->type) ) ](left, right);
	}

	Reference DECIDE(Instruction* instruction) {
		for (int i = 0; i < instruction->children.size(); i += 2) {
			if (instruction->children[i].resolve()->isTrue()) {
				return Instance->resolve(&instruction->children[i + 1].children);
			}
		}

		return Value::Empty();
	}

	Reference CALL(Instruction* instruction) {
		return Instance->call(instruction->children[0].resolve(), instruction->children[1].resolve()->as<Array&>(), true);
	}

	Reference RETURN(Instruction* instruction) {
		Instance->returned = true;
		return instruction->children[0].resolve();
	}
}

namespace Machine {
	unordered_map<Task, Reference (*)(Instruction*)> Wiring = {
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
	};

	unordered_map<Reference (*)(Instruction*), Task> Tasks = {
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
	};
}

namespace Names {
	unordered_map<Reference (*)(Instruction*), string> Wiring = {
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
		{ &Machine::RETURN, "RETURN" }
	};
}