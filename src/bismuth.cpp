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

	namespace Log {
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
	}

	static bool Exists(string path) { return !Search::Path(path).empty(); }
	static string Load(string path) {
		ifstream stream = ifstream(path);
		
		if (!stream.good()) return "";
		string script((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

		return script;
	}

	class Value;
	class Parser;

	namespace Environment { void Bind(Parser* parser); }
	typedef shared_ptr<Value> Reference;

	typedef double NumberValue;
	typedef vector<Reference> ArrayValue;
	typedef unordered_map<string, Reference> TableValue;
	typedef tuple<NumberValue, NumberValue> RangeValue;

	typedef unordered_map<string, Reference> ScopeValue;
	typedef shared_ptr<ScopeValue> Scope;

	#include "operators.cpp"
	#include "lexer.cpp"
	#include "value.cpp"
	#include "parser.cpp"
	#include "operations.cpp"
	#include "environment.cpp"
	#include "repl.cpp"
}