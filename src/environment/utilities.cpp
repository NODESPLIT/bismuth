namespace Utilities {
	void Bind(Scope environment, Parser* parser) {
		(*environment)["log"] = Value::Bound(
			[](ArrayValue arguments) {
				for (int i = 0; i < arguments.size(); i++) {
					cout << ( i == 0 ? "" : " " );
					cout << ( arguments[i]->is(Type::String) ? arguments[i]->as<string>() : arguments[i]->describe() );
				}

				cout << endl;
				return Value::Empty();
			}
		);

		(*environment)["plog"] = Value::Bound(
			[](ArrayValue arguments) {
				for (int i = 0; i < arguments.size(); i++) cout << ( arguments[i]->is(Type::String) ? arguments[i]->as<string>() : arguments[i]->describe(0) ) << endl;
				return Value::Empty();
			}
		);
	}
}