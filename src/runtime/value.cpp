#include "value.hpp"

Reference Value::Scan(Reference value, Reference name, function<Reference(Reference, Reference)> callback) {
	if (value->is(Type::Array)) {
		return Make(Scan(value->as<Array>(), callback));
	} else if (value->is(Type::Table)) {
		return Make(Scan(value->as<Table>(), callback));
	}

	return callback(value, name);
}

Array Value::Scan(Array subject, function<Reference(Reference, Reference)> callback) {
	Array output;
	for (int i = 0; i < subject.size(); i++) output.push_back(Scan(subject[i], Make(i), callback));
	return output;
}

Table Value::Scan(Table subject, function<Reference(Reference, Reference)> callback) {
	Table output;
	
	for (const auto& [ name, variable ] : subject) {
		output[name] = Empty();
		output[name]->set(Scan(variable, Make(name), callback));
	}

	return output;
}

Value::Value(Node* node) {
	if (node->mark == Mark::Word) {
		if (node->contents.compare("void") == 0) {
			type = Type::Void;
			value = nullptr;
		} else if (node->contents.compare("true") == 0) {
			type = Type::Boolean;
			value = true;
		} else if (node->contents.compare("false") == 0) {
			type = Type::Boolean;
			value = false;
		}
	} else if (node->mark == Mark::Number) {
		type = Type::Number;
		value = stod(node->contents);
	} else if (node->mark == Mark::String) {
		type = Type::String;
		value = node->contents;
	} else if (node->mark == Mark::Table) {
		type = Type::Table;
		Table table = Table{}; for (int i = 0; i < node->children.size(); i += 2) table[node->children[i].contents] = Make(&node->children[i + 1]);
		value = table;
	} else if (node->mark == Mark::Array) {
		type = Type::Array;
		Array array = Array{}; for (int i = 0; i < node->children.size(); i++) array.push_back(Make(&node->children[i]));
		value = array;
	} else if (node->mark == Mark::Block) {
		type = Type::Block;
		block = shared_ptr<Block>(new Block(node->contents));
	}
}