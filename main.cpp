#include "src/bismuth.cpp"

int main(int argc, char *argv[]) {
  vector<string> args(argv, argv + argc);

  unordered_map<string, bool> flags = {
    { "-v", false }, { "--verbose", false },
    { "-r", false }, { "--repl", false },
    { "-o", false }, { "--output", false },
    { "-t", false }, { "--test", false }
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
  bool output = flags["-o"] || flags["--output"];
  Bismuth::Verbose = flags["-v"] || flags["--verbose"];
  Bismuth::Testing = flags["-t"] || flags["--test"];

  if (repl || ( args.size() <= 1 && !Bismuth::Exists(path) )) {
    Bismuth::Repl();
  } else {
    if (Bismuth::Exists(path)) {
      Bismuth::Runtime runtime = Bismuth::Runtime(path);
      if (output && !Bismuth::Verbose && !Bismuth::Testing) cout << runtime.result->describe() << endl;
    } else if (std::filesystem::is_directory(path)) {
      vector<string> paths;

      for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path)) {
        string child = entry.path().string();
        if (Bismuth::Exists(child)) paths.push_back(child);
      }

      int count = 0;
      if (paths.size() > 1) cout << endl;
      
      for (int i = 0; i < paths.size(); i++) {
        Bismuth::Runtime runtime = Bismuth::Runtime(paths[i]);
        count += runtime.tests.size();
      }

      if (paths.size() > 1) cout << endl;
      if (count == 0) cout << "\033[91m0 tests found...\033[0m" << endl;
    } else {
      cout << "Bismuth '" << path << "' not found" << endl;
    }
  }

  return EXIT_SUCCESS;
}