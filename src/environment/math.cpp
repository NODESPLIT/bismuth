namespace Math {
	void Bind(Scope environment, Parser* parser) {
		(*environment)["math"] = Value::Empty(Type::Table);

		(*environment)["math"]->set("e", Value::Make(M_E));
		(*environment)["math"]->set("log2e", Value::Make(M_LOG2E));
		(*environment)["math"]->set("log10e", Value::Make(M_LOG10E));
		(*environment)["math"]->set("ln2", Value::Make(M_LN2));
		(*environment)["math"]->set("ln10", Value::Make(M_LN10));
		(*environment)["math"]->set("pi", Value::Make(M_PI));
		(*environment)["math"]->set("pi2", Value::Make(M_PI_2));
		(*environment)["math"]->set("pi4", Value::Make(M_PI_4));
		(*environment)["math"]->set("r1pi", Value::Make(M_1_PI));
		(*environment)["math"]->set("r2pi", Value::Make(M_2_PI));
		(*environment)["math"]->set("r2sqrtpi", Value::Make(M_2_SQRTPI));
		(*environment)["math"]->set("sqrt2", Value::Make(M_SQRT2));
		(*environment)["math"]->set("rsqrt2", Value::Make(M_SQRT1_2));

		(*environment)["math"]->set("acos", Value::Bound([](ArrayValue arguments) { return Value::Make(acos(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("asin", Value::Bound([](ArrayValue arguments) { return Value::Make(asin(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("atan", Value::Bound([](ArrayValue arguments) { return Value::Make(atan(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("atan2", Value::Bound([](ArrayValue arguments) { return Value::Make(atan2(arguments[0]->as<NumberValue>(), arguments[1]->as<NumberValue>())); }));
		(*environment)["math"]->set("cos", Value::Bound([](ArrayValue arguments) { return Value::Make(cos(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("cosh", Value::Bound([](ArrayValue arguments) { return Value::Make(cosh(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("sin", Value::Bound([](ArrayValue arguments) { return Value::Make(sin(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("sinh", Value::Bound([](ArrayValue arguments) { return Value::Make(sinh(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("tan", Value::Bound([](ArrayValue arguments) { return Value::Make(tan(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("tanh", Value::Bound([](ArrayValue arguments) { return Value::Make(tanh(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("exp", Value::Bound([](ArrayValue arguments) { return Value::Make(exp(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("ldexp", Value::Bound([](ArrayValue arguments) { return Value::Make(ldexp(arguments[0]->as<NumberValue>(), int(arguments[1]->as<NumberValue>()))); }));
		(*environment)["math"]->set("log", Value::Bound([](ArrayValue arguments) { return Value::Make(log(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("log10", Value::Bound([](ArrayValue arguments) { return Value::Make(log10(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("pow", Value::Bound([](ArrayValue arguments) { return Value::Make(pow(arguments[0]->as<NumberValue>(), arguments[1]->as<NumberValue>())); }));
		(*environment)["math"]->set("sqrt", Value::Bound([](ArrayValue arguments) { return Value::Make(sqrt(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("ceil", Value::Bound([](ArrayValue arguments) { return Value::Make(ceil(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("fabs", Value::Bound([](ArrayValue arguments) { return Value::Make(fabs(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("floor", Value::Bound([](ArrayValue arguments) { return Value::Make(floor(arguments[0]->as<NumberValue>())); }));
		(*environment)["math"]->set("fmod", Value::Bound([](ArrayValue arguments) { return Value::Make(fmod(arguments[0]->as<NumberValue>(), arguments[1]->as<NumberValue>())); }));

		(*environment)["math"]->set("frexp", Value::Bound([](ArrayValue arguments) {
			Reference output = Value::Empty(Type::Table);
			int exponent; Reference fraction = Value::Make(frexp(arguments[0]->as<NumberValue>(), &exponent));
			output->set("fraction", fraction);
			output->set("exponent", Value::Make(exponent));
			return output;
		}));

		(*environment)["math"]->set("modf", Value::Bound([](ArrayValue arguments) {
			Reference output = Value::Empty(Type::Table);
			NumberValue integer; Reference fraction = Value::Make(modf(arguments[0]->as<NumberValue>(), &integer));
			output->set("fraction", fraction);
			output->set("integer", Value::Make(integer));
			return output;
		}));
	}
}