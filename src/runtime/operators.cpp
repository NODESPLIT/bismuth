namespace Operate {
  Reference Invalid(Reference left, Reference right) { return Value::Empty(); }

  Reference NumberNumber0(Reference left, Reference right) { return Value::Make(left->as<Number>() + right->as<Number>()); };

  Reference NumberString0(Reference left, Reference right) { return Value::Make(left->describe() + right->as<String>()); };

  Reference StringString0(Reference left, Reference right) { return Value::Make(left->as<String>() + right->as<String>()); };

  Reference StringAny0(Reference left, Reference right) { return Value::Make(left->as<String>() + right->describe()); };

  Reference ArrayArray0(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	Array concat = right->as<Array>();
  	for (int i = 0; i < concat.size(); i++) array.push_back(Value::Copy(concat[i]));
  	return Value::Make(array);
  };

  Reference ArrayTable0(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	Table table = right->as<Table>();
  
  	for (const auto& [ name, variable ] : table) {
  		array.push_back(Value::Make(name));
  		array.push_back(Value::Copy(variable));
  	}
  
  	return Value::Make(array);
  };

  Reference ArrayString0(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	String string = right->as<String>();
  	for (char& character : string) array.push_back(Value::Make(String() + character));
  	return Value::Make(array);
  };

  Reference TableTable0(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	Table concat = right->as<Table>();
  	for (const auto& [ name, variable ] : concat) table[name] = Value::Copy(variable);
  	return Value::Make(table);
  };

  Reference TableArray0(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	Array array = right->as<Array>();
  	for (int i = 0; i < array.size(); i++) table[std::to_string(i)] = Value::Copy(array[i]);
  	return Value::Make(table);
  };

  Reference TableString0(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	String string = right->as<String>();
  	int i = 0; for (char& character : string) table[std::to_string(i++)] = Value::Make(String() + character);
  	return Value::Make(table);
  };

  Reference AnyString0(Reference left, Reference right) { return Value::Make(left->describe() + right->as<String>()); };

  Reference AnyAny0(Reference left, Reference right) { return Value::Make(left->describe() + right->describe()); };

  Reference NumberNumber1(Reference left, Reference right) { return Value::Make(left->as<Number>() - right->as<Number>()); };

  Reference NumberNumber2(Reference left, Reference right) { return Value::Make(left->as<Number>() * right->as<Number>()); };

  Reference StringNumber2(Reference left, Reference right) {
  	String subject = left->as<String>();
  	String output = "";
  	for (int i = 0; i < right->as<Number>(); i++) output += subject;
  	return Value::Make(output);
  };

  Reference NumberNumber3(Reference left, Reference right) { return Value::Make(left->as<Number>() / right->as<Number>()); };

  Reference StringString3(Reference left, Reference right) { return Value::Make(split(left->as<String>(), right->as<String>())); };

  Reference NumberNumber4(Reference left, Reference right) { return Value::Make(fmod(left->as<Number>(), right->as<Number>())); };

  Reference NumberAny5(Reference left, Reference right) { return Value::Make(-left->as<Number>()); };

  Reference NumberAny6(Reference left, Reference right) {
  	Reference origin = Value::Make(left->as<Number>());
  	left->set(Value::Make(left->as<Number>() + 1));
  	return origin;
  };

  Reference NumberAny7(Reference left, Reference right) {
  	Reference origin = Value::Make(left->as<Number>());
  	left->set(Value::Make(left->as<Number>() - 1));
  	return origin;
  };

  Reference NumberAny8(Reference left, Reference right) {
  	left->set(Value::Make(left->as<Number>() + 1));
  	return left;
  };

  Reference NumberAny9(Reference left, Reference right) {
  	left->set(Value::Make(left->as<Number>() - 1));
  	return left;
  };

  Reference NumberNumber10(Reference left, Reference right) { return Value::Make(left->as<Number>() < right->as<Number>()); };

  Reference NumberNumber11(Reference left, Reference right) { return Value::Make(left->as<Number>() <= right->as<Number>()); };

  Reference NumberNumber12(Reference left, Reference right) { return Value::Make(left->as<Number>() > right->as<Number>()); };

  Reference NumberNumber13(Reference left, Reference right) { return Value::Make(left->as<Number>() >= right->as<Number>()); };

  Reference AnyAny14(Reference left, Reference right) { return Value::Make(left->equals(right)); };

  Reference AnyAny15(Reference left, Reference right) { return Value::Make(!left->equals(right)); };

  Reference NumberNumber16(Reference left, Reference right) { return Value::Make(int(left->as<Number>()) & int(right->as<Number>())); };

  Reference NumberNumber17(Reference left, Reference right) { return Value::Make(int(left->as<Number>()) | int(right->as<Number>())); };

  Reference NumberNumber18(Reference left, Reference right) { return Value::Make(int(left->as<Number>()) ^ int(right->as<Number>())); };

  Reference NumberAny19(Reference left, Reference right) { return Value::Make(~int(left->as<Number>())); };

  Reference NumberNumber20(Reference left, Reference right) { return Value::Make(int(left->as<Number>()) << int(right->as<Number>())); };

  Reference NumberNumber21(Reference left, Reference right) { return Value::Make(int(left->as<Number>()) >> int(right->as<Number>())); };

  Reference NumberNumber22(Reference left, Reference right) { return Value::Make(left->as<Number>(), right->as<Number>()); };

  Reference NumberBlock23(Reference left, Reference right) {
  	Number amount = left->as<Number>();
  
  	Reference result = Value::Empty();
  	if (amount >= 0) {
  		for (int i = 0; i < amount; i++) result = Instance->call(right, { result, Value::Make(i) }, false);
  	} else {
  		for (int i = amount + 1; i >= 0; i--) result = Instance->call(right, { result, Value::Make(i) }, false);
  	}
  
  	return result;
  };

  Reference RangeBlock23(Reference left, Reference right) {
  	Range range = left->as<Range>();
  
  	Number from = get<0>(range);
  	Number to = get<1>(range);
  	
  	Reference result = Value::Empty();
  	if (from <= to) {
  		for (int i = from; i <= to; i++) result = Instance->call(right, { result, Value::Make(i) }, false);
  	} else {
  		for (int i = from; i >= to; i--) result = Instance->call(right, { result, Value::Make(i) }, false);
  	}
  
  	return result;
  };

  Reference StringBlock23(Reference left, Reference right) {
  	String value = left->as<String>();
  	Reference result = Value::Empty();
  
  	for (int i = 0; i < value.size(); i++) {
  		String character({ value.at(i) });
  		result = Instance->call(right, { result, Value::Make(character), Value::Make(i) }, false);
  	}
  
  	return result;
  };

  Reference ArrayBlock23(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Reference result = Value::Empty();
  	for (int i = 0; i < array.size(); i++) result = Instance->call(right, { result, array[i], Value::Make(i) }, false);
  
  	return result;
  };

  Reference TableBlock23(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	
  	Reference result = Value::Empty();
  	for (const auto& [ name, variable ] : table) result = Instance->call(right, { result, variable, Value::Make(name) }, false);
  
  	return result;
  };

  Reference TableBlock24(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	Reference output = Value::Empty(Type::Table);
  
  	for (const auto& [ name, variable ] : table) {
  		Reference result = Instance->call(right, { variable, Value::Make(name) }, false);
  		if (result->isFalse()) output->set(name, Value::Copy(variable));
  	}
  
  	return output;
  };

  Reference TableArray24(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	Array without = right->as<Array>();
  
  	Reference output = Value::Empty(Type::Table);
  
  	for (const auto& [ name, variable ] : table) {
  		bool removed = false;
  		
  		for (int i = 0; i < without.size(); i++) {
  			if (without[i]->as<String>().compare(name) == 0) {
  				removed = true;
  				break;
  			}
  		}
  
  		if (!removed) output->set(name, Value::Copy(variable));
  	}
  
  	return output;
  };

  Reference TableString24(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	String without = right->as<String>();
  
  	Reference output = Value::Empty(Type::Table);
  	for (const auto& [ name, variable ] : table) if (without.compare(name) != 0) output->set(name, Value::Copy(variable));
  
  	return output;
  };

  Reference ArrayBlock24(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		Reference result = Instance->call(right, { array[i], Value::Make(i) }, false);
  		if (result->isFalse()) output.push_back(Value::Copy(array[i]));
  	}
  
  	return Value::Make(output);
  };

  Reference ArrayArray24(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	Array without = right->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		bool removed = false;
  		
  		for (int w = 0; w < without.size(); w++) {
  			if (without[w]->as<Number>() == i) {
  				removed = true;
  				break;
  			}
  		}
  
  		if (!removed) output.push_back(Value::Copy(array[i]));
  	}
  
  	return Value::Make(output);
  };

  Reference ArrayNumber24(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	int without = int(right->as<Number>());
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) if (i != without) output.push_back(Value::Copy(array[i]));
  
  	return Value::Make(output);
  };

  Reference ArrayRange24(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	Range range = right->as<Range>();
  	
  	int from = int(get<0>(range));
  	int to = int(get<1>(range));
  
  	if (from < 0) from = from + array.size();
  	if (to < 0) to = to + array.size();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) if (i < from || i > to) output.push_back(Value::Copy(array[i]));
  
  	return Value::Make(output);
  };

  Reference StringBlock24(Reference left, Reference right) {
  	String string = left->as<String>();
  
  	String output;
  	for (int i = 0; i < string.size(); i++) {
  		Reference result = Instance->call(right, { Value::Make(String() + string[i]), Value::Make(i) }, false);
  		if (result->isFalse()) output += string[i];
  	}
  
  	return Value::Make(output);
  };

  Reference TableArray25(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	Array without = right->as<Array>();
  
  	Reference output = Value::Empty(Type::Table);
  	for (const auto& [ name, variable ] : table) {
  		bool removed = false;
  		
  		for (int i = 0; i < without.size(); i++) {
  			if (without[i]->equals(variable)) {
  				removed = true;
  				break;
  			}
  		}
  
  		if (!removed) output->set(name, Value::Copy(variable));
  	}
  
  	return output;
  };

  Reference TableAny25(Reference left, Reference right) {
  	Table table = left->as<Table>();
  
  	Reference output = Value::Empty(Type::Table);
  	for (const auto& [ name, variable ] : table) if (!right->equals(variable)) output->set(name, Value::Copy(variable));
  
  	return output;
  };

  Reference ArrayArray25(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	Array without = right->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		bool removed = false;
  		
  		for (int w = 0; w < without.size(); w++) {
  			if (without[w]->equals(array[i])) {
  				removed = true;
  				break;
  			}
  		}
  
  		if (!removed) output.push_back(Value::Copy(array[i]));
  	}
  
  	return Value::Make(output);
  };

  Reference ArrayAny25(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) if (!right->equals(array[i])) output.push_back(Value::Copy(array[i]));
  
  	return Value::Make(output);
  };

  Reference TableBlock26(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	Reference output = Value::Empty(Type::Table);
  
  	for (const auto& [ name, variable ] : table) {
  		Reference result = Instance->call(right, { variable, Value::Make(name) }, false);
  		if (result->isTrue()) output->set(name, Value::Copy(variable));
  	}
  
  	return output;
  };

  Reference ArrayBlock26(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		Reference result = Instance->call(right, { array[i], Value::Make(i) }, false);
  		if (result->isTrue()) output.push_back(Value::Copy(array[i]));
  	}
  
  	return Value::Make(output);
  };

  Reference StringBlock26(Reference left, Reference right) {
  	String string = left->as<String>();
  
  	String output;
  	for (int i = 0; i < string.size(); i++) {
  		Reference result = Instance->call(right, { Value::Make(String() + string[i]), Value::Make(i) }, false);
  		if (result->isTrue()) output += string[i];
  	}
  
  	return Value::Make(output);
  };

  Reference StringString27(Reference left, Reference right) {
  	vector<String> parts;
  	
  	String origin = left->as<String>();
  	for (int i = 0; i < origin.size(); i++) parts.push_back(String() + origin[i]);
  
  	return Value::Make(boost::algorithm::join(parts, right->as<String>()));
  };

  Reference ArrayString27(Reference left, Reference right) {
  	vector<String> parts;
  	
  	Array array = left->as<Array>();
  	for (int i = 0; i < array.size(); i++) parts.push_back(array[i]->cast(Type::String)->as<String>());
  
  	return Value::Make(boost::algorithm::join(parts, right->as<String>()));
  };

  Reference ArrayArray27(Reference left, Reference right) {
  	Array array = left->as<Array>();
  	Array joining = right->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		if (i > 0) for (int j = 0; j < joining.size(); j++) output.push_back(Value::Copy(joining[j]));
  		output.push_back(Value::Copy(array[i]));
  	}
  
  	return Value::Make(output);
  };

  Reference ArrayBlock27(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		if (i > 0) {
  			Reference result = Instance->call(right, { Value::Copy(array[i - 1]), Value::Copy(array[i]) }, false);
  			if (result->is(Type::Array)) {
  				Array joining = result->as<Array>();
  				for (int j = 0; j < joining.size(); j++) output.push_back(Value::Copy(joining[j]));
  			} else {
  				output.push_back(result);
  			}
  		}
  
  		output.push_back(Value::Copy(array[i]));
  	}
  
  	return Value::Make(output);
  };

  Reference ArrayAny27(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		if (i > 0) output.push_back(Value::Copy(right));
  		output.push_back(Value::Copy(array[i]));
  	}
  
  	return Value::Make(output);
  };

  Reference ArrayBlock28(Reference left, Reference right) {
  	return Value::Make(
  		Value::Scan(
  			left->as<Array>(),
  			[right](Reference value, Reference index) {
  				return Instance->call(right, { value, index }, false);
  			}
  		)
  	);
  };

  Reference TableBlock28(Reference left, Reference right) {
  	return Value::Make(
  		Value::Scan(
  			left->as<Table>(),
  			[right](Reference value, Reference key) {
  				return Instance->call(right, { value, key }, false);
  			}
  		)
  	);
  };

  Reference NumberBlock29(Reference left, Reference right) {
  	Number amount = left->as<Number>();
  
  	Array output;
  	if (amount >= 0) {
  		for (int i = 0; i < amount; i++) output.push_back(Instance->call(right, { Value::Make(i) }, false));
  	} else {
  		for (int i = amount + 1; i >= 0; i--) output.push_back(Instance->call(right, { Value::Make(i) }, false));
  	}
  
  	return Value::Make(output);
  };

  Reference RangeBlock29(Reference left, Reference right) {
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
  };

  Reference StringBlock29(Reference left, Reference right) {
  	String value = left->as<String>();
  	
  	String output = "";
  	for (int i = 0; i < value.size(); i++) {
  		String character({ value.at(i) });
  		Reference result = Instance->call(right, { Value::Make(character), Value::Make(i) }, false);
  		if (result->is(Type::String)) output += result->as<String>();
  	}
  
  	return Value::Make(output);
  };

  Reference ArrayBlock29(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		output.push_back(Instance->call(right, { array[i], Value::Make(i) }, false));
  	}
  
  	return Value::Make(output);
  };

  Reference TableBlock29(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	
  	Table output;
  	for (const auto& [ name, variable ] : table) output[name] = Instance->call(right, { variable, Value::Make(name) }, false);
  
  	return Value::Make(output);
  };

  Reference (*List[])(Reference, Reference) = { &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &NumberNumber0, &AnyAny0, &NumberString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &StringAny0, &StringAny0, &StringAny0, &StringAny0, &StringString0, &StringAny0, &StringAny0, &StringAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &ArrayString0, &ArrayArray0, &ArrayTable0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &TableString0, &TableArray0, &TableTable0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber1, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber2, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &StringNumber2, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber3, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &StringString3, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber4, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny9, &NumberAny9, &NumberAny9, &NumberAny9, &NumberAny9, &NumberAny9, &NumberAny9, &NumberAny9, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber10, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber11, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber12, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber13, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny14, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &AnyAny15, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber16, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber17, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber18, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny19, &NumberAny19, &NumberAny19, &NumberAny19, &NumberAny19, &NumberAny19, &NumberAny19, &NumberAny19, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber20, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber21, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber22, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberBlock23, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &RangeBlock23, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &StringBlock23, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &ArrayBlock23, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &TableBlock23, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &StringBlock24, &Invalid, &Invalid, &ArrayNumber24, &ArrayRange24, &Invalid, &ArrayArray24, &Invalid, &ArrayBlock24, &Invalid, &Invalid, &Invalid, &Invalid, &TableString24, &TableArray24, &Invalid, &TableBlock24, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &ArrayAny25, &ArrayAny25, &ArrayAny25, &ArrayAny25, &ArrayAny25, &ArrayArray25, &ArrayAny25, &ArrayAny25, &TableAny25, &TableAny25, &TableAny25, &TableAny25, &TableAny25, &TableArray25, &TableAny25, &TableAny25, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &StringBlock26, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &ArrayBlock26, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &TableBlock26, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &StringString27, &Invalid, &Invalid, &Invalid, &ArrayAny27, &ArrayAny27, &ArrayAny27, &ArrayAny27, &ArrayString27, &ArrayArray27, &ArrayAny27, &ArrayBlock27, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &ArrayBlock28, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &TableBlock28, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberBlock29, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &RangeBlock29, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &StringBlock29, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &ArrayBlock29, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &TableBlock29, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, };

  int Count = 8;
  int Pairs = 64;

  unordered_map<string, int> Reduced = {
    { "+", 0 },
    { "-", 64 },
    { "*", 128 },
    { "/", 192 },
    { "%", 256 },
    { "-_", 320 },
    { "_++", 384 },
    { "_--", 448 },
    { "++_", 512 },
    { "--_", 576 },
    { "<", 640 },
    { "<=", 704 },
    { ">", 768 },
    { ">=", 832 },
    { "==", 896 },
    { "!=", 960 },
    { "&", 1024 },
    { "|", 1088 },
    { "^", 1152 },
    { "~_", 1216 },
    { "<<", 1280 },
    { ">>", 1344 },
    { "->", 1408 },
    { "~:", 1472 },
    { "-:", 1536 },
    { ":-", 1600 },
    { "+:", 1664 },
    { "%:", 1728 },
    { "::", 1792 },
    { ":", 1856 },
  };
}

namespace Names {
  unordered_map<int, string> Operators = {
    { 0, "+" },
    { 64, "-" },
    { 128, "*" },
    { 192, "/" },
    { 256, "%" },
    { 320, "-_" },
    { 384, "_++" },
    { 448, "_--" },
    { 512, "++_" },
    { 576, "--_" },
    { 640, "<" },
    { 704, "<=" },
    { 768, ">" },
    { 832, ">=" },
    { 896, "==" },
    { 960, "!=" },
    { 1024, "&" },
    { 1088, "|" },
    { 1152, "^" },
    { 1216, "~_" },
    { 1280, "<<" },
    { 1344, ">>" },
    { 1408, "->" },
    { 1472, "~:" },
    { 1536, "-:" },
    { 1600, ":-" },
    { 1664, "+:" },
    { 1728, "%:" },
    { 1792, "::" },
    { 1856, ":" },
  };
}