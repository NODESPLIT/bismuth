Reference Runtime::Instruct(Instruction* instruction) { return resolve(instruction->children); }

Reference Runtime::Literal(Instruction* instruction) { return literals[instruction->index]; }

Reference Runtime::LiteralArray(Instruction* instruction) {
	Array array; for (int i = 0; i < instruction->children.size(); i++) array.push_back(resolve(&instruction->children[i]));
	return Value::Make(array);
}

Reference Runtime::LiteralTable(Instruction* instruction) {
	Table table; for (int i = 0; i < instruction->children.size(); i += 2) table[words[instruction->children[i].index]] = resolve(&instruction->children[i + 1]);
	return Value::Make(table);
}

Reference Runtime::LiteralBlock(Instruction* instruction) {
	Reference value = literals[instruction->index];
	value->block->context = current;
	return value;
}

Reference Runtime::Read(Instruction* instruction) {
	if (environment->count(words[instruction->index])) return (*environment)[words[instruction->index]];
	if (current->count(words[instruction->index]) == 0) (*current)[words[instruction->index]] = Value::Empty();
	return (*current)[words[instruction->index]];
}

Reference Runtime::Inside(Instruction* instruction) { return resolve(&instruction->children[0])->get(resolve(&instruction->children[1])); }

Reference Runtime::Define(Instruction* instruction) {
	Reference left = resolve(&instruction->children[0]);
	left->set(resolve(&instruction->children[1]));
	return left;
}

Reference Runtime::Operate(Instruction* instruction) {
	Reference left = resolve(&instruction->children[0]);
	Reference right = resolve(&instruction->children[1]);
	Transforms operations = Operate::List[instruction->index];

	if (operations.count(left->type) && operations[left->type].count(right->type)) {
		return operations[left->type][right->type](this, left, right);
	} else if (operations.count(left->type) && operations[left->type].count(Type::Any)) {
		return operations[left->type][Type::Any](this, left, right);
	} else if (operations.count(Type::Any) && operations[Type::Any].count(right->type)) {
		return operations[Type::Any][right->type](this, left, right);
	} else if (operations.count(Type::Any) && operations[Type::Any].count(Type::Any)) {
		return operations[Type::Any][Type::Any](this, left, right);
	}

	return Value::Empty();
}

Reference Runtime::Decide(Instruction* instruction) {
	for (int i = 0; i < instruction->children.size(); i += 2) {
		Reference question = resolve(&instruction->children[i]);
		if (question->isTrue()) return resolve(&instruction->children[i + 1]);
	}

	return Value::Empty();
}

Reference Runtime::Call(Instruction* instruction) {
	Reference values = resolve(&instruction->children[1]);
	return call(resolve(&instruction->children[0]), values->as<Array&>(), true);
}

Reference Runtime::Return(Instruction* instruction) {
	returned = true;
	return resolve(&instruction->children[0]);
}