#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <memory>
#include <regex>
#include <vector>
#include <unordered_map>
#include <any>

#include <ncurses.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace Bismuth {
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

	#include "log.cpp"
	#include "operators.cpp"
	#include "lexer.cpp"
	#include "value.cpp"
	#include "parser.cpp"
	#include "environment.cpp"
	#include "terminal.cpp"

	const vector<string> Parser::Extensions = { "bi", "bis" };

	void Repl(bool verbose=false) {
		Parser parser = Parser();

		Terminal terminal;
		terminal.println(string(Parser::Name) + " v" + string(Parser::Version));

		while (true) {
			string line = terminal.prompt("$>");
			if (!line.compare("exit")) break;

			Reference result = parser.run(line, verbose);
			terminal.println("\n=> " + result->describe(0));
		}
	}
}