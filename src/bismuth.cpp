#include <linenoise.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <streambuf>
#include <regex>

#include <stack>

#include <boost/container/map.hpp>
#include <boost/unordered_map.hpp>
#include <boost/container/vector.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

template <class L, class R> using unordered_map = boost::unordered_map<L, R>;
template <class L, class R> using map = boost::container::map<L, R>;
template <class T> using vector = boost::container::vector<T>;
template <class L, class R> using tuple = boost::tuple<L, R>;

using namespace std::filesystem;

using std::stack;
using std::function;
using std::string;
using std::stringstream;
using std::numeric_limits;
using std::istreambuf_iterator;
using std::ifstream;
using std::regex;
using std::cout;
using std::endl;

using boost::shared_ptr;
using boost::make_tuple;
using boost::any;

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
	#include "runtime/runtime.cpp"
	#include "runtime/compilation.cpp"
	#include "runtime/operators.cpp"
	#include "runtime/machine.cpp"

	#include "system/bindings.cpp"
	#include "system/repl.cpp"
}