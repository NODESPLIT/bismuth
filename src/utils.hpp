namespace Capture {
	std::stringstream Buffer;
	std::streambuf* Base;
	
	void Start() {
		Base = cout.rdbuf();
		cout.rdbuf(Buffer.rdbuf());
	}
	
	string Stop() {
		string output = string(Buffer.str());
		Buffer.str("");
		Buffer.clear();
		cout.rdbuf(Base);
		return output;
	}
}

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

	string Join(vector<string> parts, string delimiter="") {
		return algorithm::join(parts, delimiter);
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

	string Captured(function<void()> callback) {
		Capture::Start();
		callback();
		return Capture::Stop();
	}

	string Prefix(string content, string prefix, string title="") {
		vector<string> parts = Split(content, "\n");
		for (int i = 0; i < parts.size(); i++) parts[i] = prefix + parts[i];
		return title + Join(parts, "\n");
	}

	string Prefixed(function<void()> callback, string prefix, string title="") {
		return Prefix(Captured(callback), prefix, title);
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