namespace Operate {
	vector<Type> Typing = {
		Type::Void,
		Type::Boolean,
		Type::Number,
		Type::Range,
		Type::String,
		Type::Array,
		Type::Table,
		Type::Block
	};

	int Types = Typing.size();
	int Pairs = Types * Types;

	map<string, Transforms> NamedList = {
		#include "operators/arithmetic.cpp"
		#include "operators/comparison.cpp"
		#include "operators/inside.cpp"
	};

	Transform Invalid = [](Reference left, Reference right) { return Value::Empty(); };
	
	vector<Transform> buildList() {
		vector<Transform> list;

		for (const auto& [ name, transforms ] : NamedList) {
			for (int x = 0; x < Types; x++) {
				Type left = Typing.at(x);

				for (int y = 0; y < Types; y++) {
					Type right = Typing.at(y);
					Transform transform = Invalid;

					if (transforms.count(left) && transforms.at(left).count(right)) {
						transform = transforms.at(left).at(right);
					} else if (transforms.count(left) && transforms.at(left).count(Type::Any)) {
						transform = transforms.at(left).at(Type::Any);
					} else if (transforms.count(Type::Any) && transforms.at(Type::Any).count(right)) {
						transform = transforms.at(Type::Any).at(right);
					} else if (transforms.count(Type::Any) && transforms.at(Type::Any).count(Type::Any)) {
						transform = transforms.at(Type::Any).at(Type::Any);
					}

					list.push_back(transform);
				}
			}
		}

		return list;
	}

	unordered_map<string, int> buildReduced() {
		unordered_map<string, int> reduced; int index = 0;
		for (const auto& [ name, transforms ] : NamedList) { reduced[name] = index * Pairs; index++; }
		return reduced;
	}

	vector<Transform> List = buildList();
	unordered_map<string, int> Reduced = Operate::buildReduced();
}

namespace Names {
	vector<string> buildOperatorNames(map<string, Transforms> list) {
		vector<string> names;
		for (const auto& [ name, transforms ] : list) names.push_back(name);
		return names;
	}

	vector<string> Operators = buildOperatorNames(Operate::NamedList);
}