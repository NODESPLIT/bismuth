unordered_map<string, Operator> Operator::List = {
	{
		":",
		Operator {
			3,
			Operator::Pairing::Binary,
			Operator::Category::Inside,
			{
				{
					Number,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								NumberValue amount = left->as<NumberValue>();

								ArrayValue output;
								if (amount >= 0) {
									for (int i = 0; i < amount; i++) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								} else {
									for (int i = amount + 1; i >= 0; i--) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								}

								return Value::Make(output);
							}
						}
					}
				},
				{
					Range,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								RangeValue range = left->as<RangeValue>();

								NumberValue from = get<0>(range);
								NumberValue to = get<1>(range);
								
								ArrayValue output;
								if (from <= to) {
									for (int i = from; i <= to; i++) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								} else {
									for (int i = from; i >= to; i--) output.push_back(parser->call(right, { Value::Make(i) }, scope));
								}

								return Value::Make(output);
							}
						}
					}
				},
				{
					Array,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								ArrayValue array = left->as<ArrayValue>();

								ArrayValue output;
								for (int i = 0; i < array.size(); i++) output.push_back(parser->call(right, { array[i], Value::Make(i) }, scope));

								return Value::Make(output);
							}
						}
					}
				},
				{
					Table,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								TableValue table = left->as<TableValue>();
								
								TableValue output;
								for (const auto& [ name, variable ] : table) output[name] = parser->call(right, { variable, Value::Make(name) }, scope);

								return Value::Make(output);
							}
						}
					}
				},
				{
					String,
					{
						{
							Block,
							[](Reference left, Reference right, Parser* parser, Scope scope) {
								string value = left->as<string>();
								
								string output = "";
								for (int i = 0; i < value.size(); i++) {
									string character({ value.at(i) });
									Reference result = parser->call(right, { Value::Make(character), Value::Make(i) }, scope);
									if (result->is(String)) output += result->as<string>();
								}

								return Value::Make(output);
							}
						}
					}
				}
			}
		}
	},
	{
		"=",
		Operator {
			3,
			Operator::Pairing::Binary,
			Operator::Category::Assignment,
			{}
		}
	},
	{
		"+",
		Operator {
			1,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() + right->as<NumberValue>());
						} }
					}
				},
				{
					String,
					{
						{ String, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<string>() + right->as<string>());
						} }
					}
				}
			}
		}
	},
	{
		"-",
		Operator {
			1,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() - right->as<NumberValue>());
						} }
					}
				}
			}
		}
	},
	{
		"*",
		Operator {
			2,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() * right->as<NumberValue>());
						} }
					}
				}
			}
		}
	},
	{
		"/",
		Operator {
			2,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(left->as<NumberValue>() / right->as<NumberValue>());
						} }
					}
				}
			}
		}
	},
	{
		"%",
		Operator {
			1,
			Operator::Pairing::Binary,
			Operator::Category::Arithmetic,
			{
				{
					Number,
					{
						{ Number, [](Reference left, Reference right, Parser* parser, Scope scope) {
							return Value::Make(fmod(left->as<NumberValue>(), right->as<NumberValue>()));
						} }
					}
				}
			}
		}
	}
};