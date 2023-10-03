#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <memory>
#include <regex>
#include <vector>
#include <unordered_map>
#include <map>
#include <any>

#include <ncurses.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace Bismuth {
	const string Name = "bismuth";
	const string Version = "0.0.1";
	const string Stamp = string(Name) + " v" + string(Version);
	const vector<string> Extensions = { "bi", "bis" };
	const string Index = "index";

	bool Verbose = false;

	namespace Utils {
		regex Flatten(R"(\s+)");
		string Indent(int depth) {
			string indentation = "";
			for (int i = 0; i < depth; i++) indentation += "  ";
			return indentation;
		}
	};

	namespace Search {
		static string File(string path) {
			if (filesystem::is_regular_file(path)) return path;

			for (int i = 0; i < Extensions.size(); i++) {
				path = path + "." + Extensions[i];
				if (filesystem::is_regular_file(path)) return path;
			}

			return "";
		}

		static string Path(string path) {
			string found = File(path);
			if (!found.empty()) return found;
			if (filesystem::is_directory(path)) return File(filesystem::path(path) / Index);
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
	#include "runtime/runtime.cpp"
	#include "runtime/operators.cpp"
	#include "runtime/machine.cpp"

	#include "system/bindings.cpp"
	#include "system/repl.cpp"
}