namespace Utilities {
	void Bind(Runtime* runtime) {
		Scope environment = runtime->environment;

		(*environment)["log"] = Block::Bound(
			[](Array arguments) {
				for (int i = 0; i < arguments.size(); i++) {
					cout << ( i == 0 ? "" : " " );
					cout << ( arguments[i]->is(Type::String) ? arguments[i]->as<String>() : arguments[i]->describe() );
				}

				cout << endl;
				return Value::Empty();
			}
		);

		(*environment)["plog"] = Block::Bound(
			[](Array arguments) {
				for (int i = 0; i < arguments.size(); i++) cout << ( arguments[i]->is(Type::String) ? arguments[i]->as<String>() : arguments[i]->describe(0) ) << endl;
				return Value::Empty();
			}
		);
	}
}