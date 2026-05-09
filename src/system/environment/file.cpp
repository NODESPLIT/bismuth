namespace File {
	std::filesystem::path Base(Runtime* runtime) {
		std::filesystem::path base((*(runtime->scopes).top())[Symbol::Context]->get("path")->as<string>()); base.remove_filename();
		return base;
	}

	void Bind(Runtime* runtime) {
		Scope environment = runtime->environment;

		(*environment)["file"] = Value::Locked(Type::Table);

		(*environment)["file"]->SET("tree", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			std::filesystem::path base = Base(runtime);

			vector<string> paths;
			for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(base / path)) {
				paths.push_back(std::filesystem::relative(entry.path(), base).string());
			}

			sort(paths.begin(), paths.end());
			return Value::Make(paths);
		}));
		
		(*environment)["file"]->SET("list", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			std::filesystem::path base = Base(runtime);

			vector<string> paths;
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(base / path)) {
				paths.push_back(std::filesystem::relative(entry.path(), base).string());
			}

			sort(paths.begin(), paths.end());
			return Value::Make(paths);
		}));
	}
}