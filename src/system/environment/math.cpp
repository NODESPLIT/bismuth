namespace Math {
	void Bind(Runtime* runtime) {
		Scope environment = runtime->environment;

		(*environment)["math"] = Value::Locked(Type::Table);

		(*environment)["math"]->SET("e", Value::Make(M_E));
		(*environment)["math"]->SET("log2e", Value::Make(M_LOG2E));
		(*environment)["math"]->SET("log10e", Value::Make(M_LOG10E));
		(*environment)["math"]->SET("ln2", Value::Make(M_LN2));
		(*environment)["math"]->SET("ln10", Value::Make(M_LN10));
		(*environment)["math"]->SET("pi", Value::Make(M_PI));
		(*environment)["math"]->SET("pi2", Value::Make(M_PI_2));
		(*environment)["math"]->SET("pi4", Value::Make(M_PI_4));
		(*environment)["math"]->SET("r1pi", Value::Make(M_1_PI));
		(*environment)["math"]->SET("r2pi", Value::Make(M_2_PI));
		(*environment)["math"]->SET("r2sqrtpi", Value::Make(M_2_SQRTPI));
		(*environment)["math"]->SET("sqrt2", Value::Make(M_SQRT2));
		(*environment)["math"]->SET("rsqrt2", Value::Make(M_SQRT1_2));

		(*environment)["math"]->SET("acos", Block::Bound([](Array arguments) { return Value::Make(acos(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("asin", Block::Bound([](Array arguments) { return Value::Make(asin(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("atan", Block::Bound([](Array arguments) { return Value::Make(atan(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("atan2", Block::Bound([](Array arguments) { return Value::Make(atan2(arguments[0]->as<Number>(), arguments[1]->as<Number>())); }));
		(*environment)["math"]->SET("cos", Block::Bound([](Array arguments) { return Value::Make(cos(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("cosh", Block::Bound([](Array arguments) { return Value::Make(cosh(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("sin", Block::Bound([](Array arguments) { return Value::Make(sin(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("sinh", Block::Bound([](Array arguments) { return Value::Make(sinh(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("tan", Block::Bound([](Array arguments) { return Value::Make(tan(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("tanh", Block::Bound([](Array arguments) { return Value::Make(tanh(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("exp", Block::Bound([](Array arguments) { return Value::Make(exp(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("ldexp", Block::Bound([](Array arguments) { return Value::Make(ldexp(arguments[0]->as<Number>(), int(arguments[1]->as<Number>()))); }));
		(*environment)["math"]->SET("log", Block::Bound([](Array arguments) { return Value::Make(log(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("log10", Block::Bound([](Array arguments) { return Value::Make(log10(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("pow", Block::Bound([](Array arguments) { return Value::Make(pow(arguments[0]->as<Number>(), arguments[1]->as<Number>())); }));
		(*environment)["math"]->SET("sqrt", Block::Bound([](Array arguments) { return Value::Make(sqrt(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("ceil", Block::Bound([](Array arguments) { return Value::Make(ceil(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("fabs", Block::Bound([](Array arguments) { return Value::Make(fabs(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("floor", Block::Bound([](Array arguments) { return Value::Make(floor(arguments[0]->as<Number>())); }));
		(*environment)["math"]->SET("fmod", Block::Bound([](Array arguments) { return Value::Make(fmod(arguments[0]->as<Number>(), arguments[1]->as<Number>())); }));

		(*environment)["math"]->SET("frexp", Block::Bound([](Array arguments) {
			Reference output = Value::Locked(Type::Table);
			int exponent; Reference fraction = Value::Make(frexp(arguments[0]->as<Number>(), &exponent));
			output->SET("fraction", fraction);
			output->SET("exponent", Value::Make(exponent));
			return output;
		}));

		(*environment)["math"]->SET("modf", Block::Bound([](Array arguments) {
			Reference output = Value::Locked(Type::Table);
			Number integer; Reference fraction = Value::Make(modf(arguments[0]->as<Number>(), &integer));
			output->SET("fraction", fraction);
			output->SET("integer", Value::Make(integer));
			return output;
		}));
	}
}