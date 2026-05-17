namespace Machine {
	Reference VALUE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		return Instance->literals[instruction->mode];
	}

	Reference ARRAY(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		Reference array = Value::Empty(Type::Array);
		vector<int>* relations = &Instance->relations[instruction->children[0]];
		for (int i = 0; i < relations->size(); i++) array->set(i, Value::Copy(state[ (*relations)[i] ]));
		return array;
	}

	Reference TABLE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		Reference table = Value::Empty(Type::Table);
		vector<int>* relations = &Instance->relations[instruction->children[0]];
		
		for (int i = 0; i < relations->size(); i += 2) {
			Reference value = Value::Copy(state[ (*relations)[i + 1] ]);
			table->set(
				state[ (*relations)[i] ]->as<String>(),
				value
			);
		}

		return table;
	}

	Reference BLOCK(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		Reference value = Value::Copy(Instance->literals[instruction->mode]);
		value->context = Instance->scopes.top();
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
		Reference table = state[instruction->children[0]];
		Reference key = state[instruction->children[1]];

		Reference result = table->get(key);
		result->container = table;
		
		if (result->is(Type::Void)) {
			Reference hook = table->meta("index");
			hook->container = table;
			if (hook->is(Type::Block)) return Instance->call(hook, { key }, true);
		}

		return result;
	}

	Reference DEFINE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		Reference left = state[instruction->children[0]];
		Reference value = Value::Copy(instruction->children[1] > -1 ? state[instruction->children[1]] : last);
		left->set(value);
		return left;
	}

	Reference DELETE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor) {
		string word = Instance->words[instruction->mode];
		(*Instance->scopes.top()).erase(word);
		return last;
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
		return Instance->call(state[instruction->children[0]], *(state[instruction->children[1]]->point<Array>()), true);
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
		{ Task::Delete, &Machine::DELETE },
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
		{ &Machine::DELETE, Task::Delete },
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
		{ &Machine::DELETE, "DELETE" },
		{ &Machine::OPERATE, "OPERATE" },
		{ &Machine::DECIDE, "DECIDE" },
		{ &Machine::CALL, "CALL" },
		{ &Machine::RETURN, "RETURN" },
		{ nullptr, "JUMP" }
	};
}