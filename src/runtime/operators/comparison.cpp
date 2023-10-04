{
	"<",
	{ { Type::Number, { { Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() < right->as<Number>()); } } } } }
},
{
	"<=",
	{ { Type::Number, { { Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() <= right->as<Number>()); } } } } }
},
{
	">",
	{ { Type::Number, { { Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() > right->as<Number>()); } } } } }
},
{
	">=",
	{ { Type::Number, { { Type::Number, [](Reference left, Reference right) { return Value::Make(left->as<Number>() >= right->as<Number>()); } } } } }
},
{
	"==",
	{ { Type::Any, { { Type::Any, [](Reference left, Reference right) { return Value::Make(left->equals(right)); } } } } }
},
{
	"!=",
	{ { Type::Any, { { Type::Any, [](Reference left, Reference right) { return Value::Make(!left->equals(right)); } } } } }
},