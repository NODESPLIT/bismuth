namespace Filesystem {
	std::filesystem::path Base(Runtime* runtime) {
		std::filesystem::path base((*(runtime->scopes).top())[Symbol::Context]->get("path")->as<string>()); base.remove_filename();
		return base;
	}

	void Bind(Runtime* runtime) {
		Scope environment = runtime->environment;

		(*environment)["filesystem"] = Value::Locked(Type::Table);

		(*environment)["filesystem"]->SET("tree", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			std::filesystem::path base = Base(runtime);

			vector<string> paths;
			for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(base / path)) {
				paths.push_back(std::filesystem::relative(entry.path(), base).string());
			}

			sort(paths.begin(), paths.end());
			return Value::Make(paths);
		}));
		
		(*environment)["filesystem"]->SET("list", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			std::filesystem::path base = Base(runtime);

			vector<string> paths;
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(base / path)) {
				paths.push_back(std::filesystem::relative(entry.path(), base).string());
			}

			sort(paths.begin(), paths.end());
			return Value::Make(paths);
		}));
		
		(*environment)["filesystem"]->SET("file", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			return Value::Make(std::filesystem::is_regular_file(Base(runtime) / path));
		}));
		
		(*environment)["filesystem"]->SET("folder", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			return Value::Make(std::filesystem::is_directory(Base(runtime) / path));
		}));
		
		(*environment)["filesystem"]->SET("path", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			return Value::Make(std::filesystem::exists(Base(runtime) / path));
		}));
		
		(*environment)["filesystem"]->SET("read", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			
			string output = "";
			std::ifstream file(Base(runtime) / path);
			while (getline(file, output));
			file.close();
			
			return Value::Make(output);
		}));
		
		(*environment)["filesystem"]->SET("write", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			
			string content = arguments[1]->as<string>();
			std::ofstream file(Base(runtime) / path);
			file << content.c_str();
			file.close();
			
			return Value::Make(true);
		}));
		
		(*environment)["filesystem"]->SET("build", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			return Value::Make(std::filesystem::create_directories(Base(runtime) / path));
		}));
		
		(*environment)["filesystem"]->SET("delete", Block::Bound([=](Array arguments) {
			string path = arguments[0]->as<string>();
			uintmax_t errors = std::filesystem::remove_all(Base(runtime) / path);
			return Value::Make(errors > 0);
		}));
	}
}