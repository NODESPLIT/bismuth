namespace Time {
	double Stamp() { return double(duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()); }
	void Bind(Scope environment, Parser* parser) {
		(*environment)["time"] = Value::Empty(Type::Table);
		(*environment)["time"]->set("now", Value::Bound([](ArrayValue arguments) { return Value::Make(Stamp()); }));
		(*environment)["time"]->set("while", Value::Bound([parser](ArrayValue arguments) {
			double stamp = Stamp();
			parser->call(arguments[0]);
			return Value::Make(Stamp() - stamp);
		}));
	}
}