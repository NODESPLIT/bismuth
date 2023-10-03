namespace Time {
	double Stamp() { return double(duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()); }
	void Bind(Runtime* runtime) {
		Scope environment = runtime->environment;

		(*environment)["time"] = Value::Locked(Type::Table);
		(*environment)["time"]->SET("now", Block::Bound([](Array arguments) { return Value::Make(Stamp()); }));
		
		(*environment)["time"]->SET("while", Block::Bound([runtime](Array arguments) {
			double start = Stamp();
			runtime->call(arguments[0]);
			double end = Stamp();
			return Value::Make(end - start);
		}));
	}
}