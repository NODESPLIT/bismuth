#include <string>
#include <vector>

using namespace std;

#include "src/bismuth.cpp"

int main(int argc, char *argv[]) {
	vector<string> args(argv, argv + argc);

	unordered_map<string, bool> flags = {
		{ "-v", false }, { "--verbose", false },
		{ "-r", false }, { "--repl", false },
		{ "-q", false }, { "--quiet", false }
	};

	string path = "./";
	for (int i = 1; i < args.size(); i++) {
		if (flags.count(args[i])) {
			flags[args[i]] = true;
		} else {
			path = args[i];
		}
	}

	bool repl = flags["-r"] || flags["--repl"];
	bool quiet = flags["-q"] || flags["--quiet"];
	bool verbose = flags["-v"] || flags["--verbose"];

	if (repl || ( args.size() <= 1 && !Bismuth::Exists(path) )) {
		Bismuth::Repl();
	} else {
		if (Bismuth::Exists(path)) {
			Bismuth::Parser parser = Bismuth::Parser(path, verbose);
			if (!quiet && !verbose) cout << parser.result->describe() << endl;
		} else {
			cout << "File '" << path << "' not found" << endl;
		}
	}

	return EXIT_SUCCESS;
}