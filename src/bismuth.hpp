#include <string>
#include <regex>
#include <stack>

#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/unordered_map.hpp>
#include <tsl/ordered_map.h>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

namespace algorithm = boost::algorithm;

template <class L, class R> using unordered_map = boost::unordered_map<L, R>;
template <class T> using vector = boost::container::vector<T>;
template <class L, class R> using tuple = boost::tuple<L, R>;

using std::function;

using std::string;
using std::stringstream;
using std::numeric_limits;
using std::regex;
using std::stack;

using boost::make_tuple;
using boost::shared_ptr;
using boost::any;

namespace Utils {
	regex Flatten(R"(\s+)");
	
	string Indent(int depth) {
		string indentation = "";
		for (int i = 0; i < depth; i++) indentation += "  ";
		return indentation;
	}

	vector<string> Split(string text, string search) {
		vector<string> parts;
		algorithm::split(parts, text, algorithm::is_any_of(search));
		return parts;
	};

	string TrimLeft(string text) {
		boost::trim_left(text);
		return text;
	}

	string TrimRight(string text) {
		boost::trim_right(text);
		return text;
	}

	string Trim(string text) {
		return TrimRight(TrimLeft(text));
	}

	string Unindent(string text) {
		string output = "";
		vector<string> lines = Split(text, "\n");
		
		for (int i = 0; i < lines.size(); i++) {
			if (i > 0) output += "\n";
			output += Trim(lines[i]);
		}

		return output;
	}

	string Boxed(string text, int width, int indent = 1, bool titled = false, bool attached = false) {
		string output = "";
		vector<string> parts = Split("│" + TextFlow::Column(text).width(width).indent(indent).toString(), "\n");

		for (int i = 0; i < parts.size(); i++) {
			string part = parts[i];
			string edge = "";
			for (int s = 0; s < (width + indent + (i == 0 ? 3 : 0)) - part.size(); s++) edge += " ";
			if (i == 0) part = "\033[1m" + parts[i] + "\033[0m";
			output += part + edge + "│" + (( i == parts.size() - 1 ) ? "" : "\n│");
		}

		output = "┐\n" + output;
		
		for (int i = 0; i <= width; i++) output = "─" + output;
		output = "┌" + output;

		output += string("\n") + ( attached ? "├" : "└" );
		for (int i = 0; i <= width; i++) output += "─";
		output += "┘";

		return output;
	}
};