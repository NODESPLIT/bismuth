struct Symbol {
	inline static const string Context = "#";
	inline static const string Instance = "@";
};

class Runtime {
	public:
		static void Log(Scope scope, int depth=0);

		vector<string> words;
		vector<Reference> literals;
		vector<vector<int>> relations;

		stack<Scope> scopes;
		Scope environment = Scope(new Table());
		Scope global = Scope(new Table());

		Reference result;
		bool returned = false;

		Runtime();
		Runtime(string path, Reference in=nullptr);

		Reference load(string path, Reference in=nullptr, Scope scope=nullptr);
		Reference import(string path, Reference in=nullptr);
		Reference interpret(string bismuth);
		Reference interpret(vector<Token>* tokens);
		Reference interpret(vector<Node>* tree);

		void init(Scope scope, string path="./", Reference in=nullptr);
		Scope branched(Scope origin=nullptr, Table initial={});

		Reference run(vector<Instruction> instructions, bool returns=false) {
			// cout << "Running:" << endl;
			// Machine::Log(this, &instructions);

			Reference state[instructions.size()];
			Reference last = Value::Empty();

			Cursor cursor;
			while (cursor.position < instructions.size()) {
				// cout << "CURSOR: " << cursor.position << endl;
				Instruction* instruction = &instructions[cursor.position];

				if (instruction->wire) {
					last = instruction->wire(state, instruction, last, &cursor);
					state[cursor.position] = last;
					if (returned) {
						if (returns) returned = false;
						return last;
					}
				} else {
					cursor.jump = instruction->mode;
				}

				if (cursor.go > -1) {
					cursor.position = cursor.go;
					cursor.go = -1;
				} else {
					cursor.position += cursor.jump + 1;
					cursor.jump = 0;
				}
			}

			return last;
		}

		Reference call(Reference value, bool branch=true) { return call(value, {}, branch); }
		Reference call(Reference value, Array values, bool branch=true) {
			shared_ptr<Block> block = value->block;
			if (block->binding) return block->binding(values);

			scopes.push(branch ? branched(value->context) : value->context);

			Reference arguments = Value::Lock(Value::Empty(Type::Array));
			int argument = 0;

			for (int i = 0; i < block->arguments.size(); i++) {
				if (i >= values.size() || values[i]->is(Type::Void)) {
					Reference defaulting = run(block->defaults[i]);
					(*scopes.top())[block->arguments[i]] = defaulting;
					arguments->SET(argument, Value::Copy(defaulting));
				} else {
					(*scopes.top())[block->arguments[i]] = Value::Copy(values[i]);
					arguments->SET(argument, Value::Copy(values[i]));
				}

				argument++;
			}

			for (int i = argument; i < values.size(); i++) arguments->SET(i, Value::Copy(values[i]));
			(*scopes.top())[Symbol::Context]->SET("args", arguments);

			if (value->container) (*scopes.top())[Symbol::Instance] = value->container;

			Reference result = run(block->body, branch);
			scopes.pop();

			return Value::Copy(result);
		}
};