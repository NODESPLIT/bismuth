namespace System {
	void Bind(Runtime* runtime) {
		Scope environment = runtime->environment;
		(*environment)["import"] = Block::Bound([runtime](Array arguments) {
			return runtime->import(arguments[0]->as<String>(), arguments.size() >= 2 ? arguments[1] : nullptr);
		});
	}
}