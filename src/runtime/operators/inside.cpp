{
	":",
	{
		{
			Type::Number,
			{
				{
					Type::Block,
					[](Reference left, Reference right) {
						Number amount = left->as<Number>();

						Array output;
						if (amount >= 0) {
							for (int i = 0; i < amount; i++) output.push_back(Instance->call(right, { Value::Make(i) }, false));
						} else {
							for (int i = amount + 1; i >= 0; i--) output.push_back(Instance->call(right, { Value::Make(i) }, false));
						}

						return Value::Make(output);
					}
				}
			}
		},
		{
			Type::Range,
			{
				{
					Type::Block,
					[](Reference left, Reference right) {
						Range range = left->as<Range>();

						Number from = get<0>(range);
						Number to = get<1>(range);
						
						Array output;
						if (from <= to) {
							for (int i = from; i <= to; i++) output.push_back(Instance->call(right, { Value::Make(i) }, false));
						} else {
							for (int i = from; i >= to; i--) output.push_back(Instance->call(right, { Value::Make(i) }, false));
						}

						return Value::Make(output);
					}
				}
			}
		},
		{
			Type::Array,
			{
				{
					Type::Block,
					[](Reference left, Reference right) {
						Array array = left->as<Array>();

						Array output;
						for (int i = 0; i < array.size(); i++) {
							output.push_back(Instance->call(right, { array[i], Value::Make(i) }, false));
						}

						return Value::Make(output);
					}
				}
			}
		},
		{
			Type::Table,
			{
				{
					Type::Block,
					[](Reference left, Reference right) {
						Table table = left->as<Table>();
						
						Table output;
						for (const auto& [ name, variable ] : table) output[name] = Instance->call(right, { variable, Value::Make(name) }, false);

						return Value::Make(output);
					}
				}
			}
		},
		{
			Type::String,
			{
				{
					Type::Block,
					[](Reference left, Reference right) {
						String value = left->as<String>();
						
						String output = "";
						for (int i = 0; i < value.size(); i++) {
							String character({ value.at(i) });
							Reference result = Instance->call(right, { Value::Make(character), Value::Make(i) }, false);
							if (result->is(Type::String)) output += result->as<String>();
						}

						return Value::Make(output);
					}
				}
			}
		}
	}
},
{
	"->",
	{
		{
			Type::Number,
			{
				{
					Type::Number,
					[](Reference left, Reference right) {
						return Value::Make(left->as<Number>(), right->as<Number>());
					}
				}
			}
		}
	}
},