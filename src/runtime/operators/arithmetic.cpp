{
	"+",
	{
		{
			Type::Number,
			{
				{ Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() + right->as<Number>()); } },
				{ Type::String, [](Reference left, Reference right) { return Value::Make(left->describe() + right->as<String>()); } }
			}
		},
		{
			Type::String,
			{
				{ Type::String, [](Reference left, Reference right) { return Value::Make(left->as<String>() + right->as<String>()); } },
				{ Type::Any, [](Reference left, Reference right) { return Value::Make(left->as<String>() + right->describe()); } }
			}
		},
		{
			Type::Any,
			{
				{ Type::String, [](Reference left, Reference right) { return Value::Make(left->describe() + right->as<String>()); } },
				{ Type::Any, [](Reference left, Reference right) { return Value::Make(left->describe() + right->describe()); } }
			}
		},
	}
},
{
	"-",
	{
		{
			Type::Number,
			{ { Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() - right->as<Number>()); } } }
		}
	}
},
{
	"*",
	{
		{
			Type::Number,
			{ { Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() * right->as<Number>()); } } }
		}
	}
},
{
	"/",
	{
		{
			Type::Number,
			{ { Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() / right->as<Number>()); } } }
		}
	}
},
{
	"%",
	{
		{
			Type::Number,
			{ { Type::Number, [](Reference left, Reference right) { return Value::Make(fmod(left->as<Number>(), right->as<Number>())); } } }
		}
	}
},
{
	"%",
	{
		{
			Type::Number,
			{ { Type::Number, [](Reference left, Reference right) { return Value::Make(fmod(left->as<Number>(), right->as<Number>())); } } }
		}
	}
},
{
	"-_",
	{
		{
			Type::Number,
			{ { Type::Any, [](Reference left, Reference right) { return Value::Make(-left->as<Number>()); } } }
		}
	}
},
{
	"_++",
	{
		{
			Type::Number,
			{ { Type::Any, [](Reference left, Reference right) {
				Reference origin = Value::Make(left->as<Number>());
				left->set(Value::Make(left->as<Number>() + 1));
				return origin;
			} } }
		}
	}
},
{
	"_--",
	{
		{
			Type::Number,
			{ { Type::Any, [](Reference left, Reference right) {
				Reference origin = Value::Make(left->as<Number>());
				left->set(Value::Make(left->as<Number>() - 1));
				return origin;
			} } }
		}
	}
},
{
	"++_",
	{
		{
			Type::Number,
			{ { Type::Any, [](Reference left, Reference right) {
				left->set(Value::Make(left->as<Number>() + 1));
				return left;
			} } }
		}
	}
},
{
	"--_",
	{
		{
			Type::Number,
			{ { Type::Any, [](Reference left, Reference right) {
				left->set(Value::Make(left->as<Number>() - 1));
				return left;
			} } }
		}
	}
},