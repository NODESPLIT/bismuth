namespace Primatives {
	void Bind(Runtime* runtime) {
		Scope environment = runtime->environment;
		(*environment)["Void"] = Block::Bound([](Array arguments) { return Value::Empty(); });
		(*environment)["Boolean"] = Block::Bound([](Array arguments) { return arguments[0]->cast(Type::Boolean); });
		(*environment)["Number"] = Block::Bound([](Array arguments) { return arguments[0]->cast(Type::Number); });
		(*environment)["Range"] = Block::Bound([](Array arguments) { return arguments[0]->cast(Type::Range); });
		(*environment)["String"] = Block::Bound([](Array arguments) { return arguments[0]->cast(Type::String); });
		(*environment)["Array"] = Block::Bound([](Array arguments) { return arguments[0]->cast(Type::Array); });
		(*environment)["Table"] = Block::Bound([](Array arguments) { return arguments[0]->cast(Type::Table); });
		(*environment)["Block"] = Block::Bound([](Array arguments) { return arguments[0]->cast(Type::Block); });
	}
}