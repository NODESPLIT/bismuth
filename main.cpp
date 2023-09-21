#include <string>
#include <vector>

using namespace std;

#include "src/bismuth.cpp"

int main(int argc, char *argv[]) {
	vector<string> args(argv, argv + argc);

	if (args.size() >= 2) {
		unordered_map<string, bool> flags = { { "-v", false }, { "verbose", false } };
		for (int i = 1; i < args.size() - 1; i++) if (flags.count(args[i])) flags[args[i]] = true;
		Bismuth::Parser parser = Bismuth::Parser(args[args.size() - 1], flags["-v"] || flags["verbose"]);
	} else {
		Bismuth::Repl();
	}

	return 0;
}