namespace Operate {
  Reference Invalid(Reference left, Reference right) { return Value::Empty(); }

  Reference NumberNumber0(Reference left, Reference right) { return Value::Make(left->as<Number>() + right->as<Number>()); };

  Reference NumberString0(Reference left, Reference right) { return Value::Make(left->describe() + right->as<String>()); };

  Reference StringString0(Reference left, Reference right) { return Value::Make(left->as<String>() + right->as<String>()); };

  Reference StringAny0(Reference left, Reference right) { return Value::Make(left->as<String>() + right->describe()); };

  Reference AnyString0(Reference left, Reference right) { return Value::Make(left->describe() + right->as<String>()); };

  Reference AnyAny0(Reference left, Reference right) { return Value::Make(left->describe() + right->describe()); };

  Reference NumberNumber1(Reference left, Reference right) { return Value::Make(left->as<Number>() - right->as<Number>()); };

  Reference NumberNumber2(Reference left, Reference right) { return Value::Make(left->as<Number>() * right->as<Number>()); };

  Reference NumberNumber3(Reference left, Reference right) { return Value::Make(left->as<Number>() / right->as<Number>()); };

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

  Reference NumberNumber9(Reference left, Reference right) { return Value::Make(left->as<Number>() < right->as<Number>()); };

  Reference NumberNumber10(Reference left, Reference right) { return Value::Make(left->as<Number>() <= right->as<Number>()); };

  Reference NumberNumber11(Reference left, Reference right) { return Value::Make(left->as<Number>() > right->as<Number>()); };

  Reference NumberNumber12(Reference left, Reference right) { return Value::Make(left->as<Number>() >= right->as<Number>()); };

  Reference AnyAny13(Reference left, Reference right) { return Value::Make(left->equals(right)); };

  Reference NumberNumber14(Reference left, Reference right) { return Value::Make(left->as<Number>(), right->as<Number>()); };

  Reference NumberBlock15(Reference left, Reference right) {
  	Number amount = left->as<Number>();
  
  	Array output;
  	if (amount >= 0) {
  		for (int i = 0; i < amount; i++) output.push_back(Instance->call(right, { Value::Make(i) }, false));
  	} else {
  		for (int i = amount + 1; i >= 0; i--) output.push_back(Instance->call(right, { Value::Make(i) }, false));
  	}
  
  	return Value::Make(output);
  };

  Reference RangeBlock15(Reference left, Reference right) {
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

  Reference ArrayBlock15(Reference left, Reference right) {
  	Array array = left->as<Array>();
  
  	Array output;
  	for (int i = 0; i < array.size(); i++) {
  		output.push_back(Instance->call(right, { array[i], Value::Make(i) }, false));
  	}
  
  	return Value::Make(output);
  };

  Reference TableBlock15(Reference left, Reference right) {
  	Table table = left->as<Table>();
  	
  	Table output;
  	for (const auto& [ name, variable ] : table) output[name] = Instance->call(right, { variable, Value::Make(name) }, false);
  
  	return Value::Make(output);
  };

  Reference (*List[])(Reference, Reference) = { &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &NumberNumber0, &AnyAny0, &NumberString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &StringAny0, &StringAny0, &StringAny0, &StringAny0, &StringString0, &StringAny0, &StringAny0, &StringAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyAny0, &AnyString0, &AnyAny0, &AnyAny0, &AnyAny0, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber1, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber2, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber3, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber4, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &NumberAny5, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &NumberAny6, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &NumberAny7, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &NumberAny8, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber9, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber10, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber11, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber12, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &AnyAny13, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberNumber14, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &NumberBlock15, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &RangeBlock15, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &ArrayBlock15, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &TableBlock15, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, &Invalid, };

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
    { "<", 576 },
    { "<=", 640 },
    { ">", 704 },
    { ">=", 768 },
    { "==", 832 },
    { "->", 896 },
    { ":", 960 },
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
    { 576, "<" },
    { 640, "<=" },
    { 704, ">" },
    { 768, ">=" },
    { 832, "==" },
    { 896, "->" },
    { 960, ":" },
  };
}