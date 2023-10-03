namespace Execute {
	Reference String(Runtime* runtime, string bismuth, string path="", Reference in=nullptr) {
		if (Bismuth::Verbose) {
			cout << "\n\n--- Bismuth[" << runtime->path << "] ---" << endl;
			cout << endl << bismuth << endl;
		}

		vector<Token> tokens;
		Analyse(bismuth, &tokens);

		if (Bismuth::Verbose) {
			cout << "\n\n--- Tokens ---\n" << endl;
			Token::Log(&tokens, runtime->depth);
		}
		
		vector<Node> tree;
		Parse(&tokens, &tree);

		if (Bismuth::Verbose) {
			cout << "\n\n--- Tree ---\n" << endl;
			Node::Log(&tree, runtime->depth);
			cout << "\n\n--- Instructions ---\n" << endl;
		}
		
		Reference result = runtime->interpret(&tree);

		if (Bismuth::Verbose) {
			cout << "\n\n--- Result ---\n" << endl;
			cout << result->describe(runtime->depth) << endl << endl;
			cout << "\n--- Globals ---\n" << endl;
			Runtime::Log(runtime->global, runtime->depth); cout << endl << endl;
		}
		
		return result;
	}

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

	Reference File(string path, Reference in=nullptr, Runtime* parent=nullptr) {
		if (parent) {
			filesystem::path base(parent->path);
			base.remove_filename();
			path = base / path;
		}

		ifstream stream = ifstream(Search::Path(path));
			
		if (!stream.good()) return Value::Empty();
		string bismuth((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

		Runtime runtime = Runtime(path, in, parent);
		return String(&runtime, bismuth, path, in);
	}
}

static bool Exists(string path) {
	return !Execute::Search::Path(path).empty();
}