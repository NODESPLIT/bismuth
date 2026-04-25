#include <linenoise.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <filesystem>

#include <boost/dll.hpp>

#include <boost/tuple/tuple_comparison.hpp>
#include <boost/make_shared.hpp>

using std::istreambuf_iterator;
using std::ifstream;
using std::cout;
using std::endl;

#include "bismuth.hpp"

vector<string> split(string text, string search) {
	vector<string> parts;
	algorithm::split(parts, text, algorithm::is_any_of(search));
	return parts;
};

namespace Bismuth {
	const string Name = "bismuth";
	const string Version = "0.0.1";
	const string Stamp = string(Name) + " v" + string(Version);
	const vector<string> Extensions = { "bi", "bis" };
	const string Index = "index";

	bool Verbose = false;

	namespace Search {
		static string File(string path) {
			if (std::filesystem::is_regular_file(path)) return path;

			for (int i = 0; i < Extensions.size(); i++) {
				path = path + "." + Extensions[i];
				if (std::filesystem::is_regular_file(path)) return path;
			}

			return "";
		}

		static string Path(string path) {
			string found = File(path);
			if (!found.empty()) return found;
			if (std::filesystem::is_directory(path)) return File(std::filesystem::path(path) / Index);
			return "";
		}
	};

	static bool Exists(string path) {
		return !Search::Path(path).empty();
	}

	#include "grammar/mark.cpp"
	#include "grammar/type.cpp"
	#include "grammar/operators.cpp"
	#include "grammar/task.cpp"

	#include "lexing/token.cpp"
	#include "lexing/scanning.cpp"
	#include "lexing/analysis.cpp"

	#include "parsing/node.cpp"
	#include "parsing/symbols.cpp"
	#include "parsing/parser.cpp"

	#include "runtime/value.cpp"
	#include "runtime/instruction.cpp"
	#include "crystal/api.hpp"
	#include "runtime/runtime.hpp"
	#include "runtime/runtime.cpp"
	#include "runtime/compilation.cpp"
	#include "runtime/operators.cpp"
	#include "runtime/machine.cpp"

	#include "system/bindings.cpp"
	#include "system/repl.cpp"
}