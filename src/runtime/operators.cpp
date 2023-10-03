namespace Operate {
	map<string, Transforms> NamedList = {
		#include "operators/arithmetic.cpp"
		#include "operators/comparison.cpp"
		#include "operators/inside.cpp"
	};
	
	vector<Transforms> buildList() {
		vector<Transforms> list;
		for (const auto& [ name, transforms ] : NamedList) list.push_back(transforms);
		return list;
	}

	unordered_map<string, int> buildReduced() {
		unordered_map<string, int> reduced; int index = 0;
		for (const auto& [ name, transforms ] : NamedList) { reduced[name] = index; index++; }
		return reduced;
	}

	vector<Transforms> List = Operate::buildList();
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