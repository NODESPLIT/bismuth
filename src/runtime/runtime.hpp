struct Symbol {
	inline static const string Context = "#";
	inline static const string Instance = "@";
};

struct State {
	vector<Annotated>* instructions;
	Cursor cursor;
	Annotated* instruction() { return &(*instructions)[cursor.position]; }
};

struct Test {
	string description;
	bool passed;
};

class Runtime {
	public:
		static void Log(Scope scope, int depth=0);
		static inline State Current;
		static inline vector<Test> Tests;

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

		Reference run(vector<Annotated> annotated, bool returns=false) {
			// cout << "Running:" << endl;
			// Machine::Log(this, &instructions);

			State saved = Current;
			Current = State(&annotated);

			vector<Instruction> instructions;
			for (int i = 0; i < annotated.size(); i++) instructions.push_back(annotated[i]);

			Reference state[instructions.size()];
			Reference last = Value::Empty();

			while (Current.cursor.position < instructions.size()) {
				// cout << "CURSOR: " << Current.cursor.position << endl;
				Instruction* instruction = &instructions[Current.cursor.position];

				if (instruction->wire) {
					last = instruction->wire(state, instruction, last, &Current.cursor);
					state[Current.cursor.position] = last;
					if (returned) {
						if (returns) returned = false;
						return last;
					}
				} else {
					Current.cursor.jump = instruction->mode;
				}

				if (Current.cursor.go > -1) {
					Current.cursor.position = Current.cursor.go;
					Current.cursor.go = -1;
				} else {
					Current.cursor.position += Current.cursor.jump + 1;
					Current.cursor.jump = 0;
				}
			}

			Current = saved;
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
			(*scopes.top())[Symbol::Context]->SET("called", value);

			Reference call = Value::Lock(Value::Empty(Type::Table));
			call->descriptor = Current.instruction()->descriptor;
			(*scopes.top())[Symbol::Context]->SET("call", call);

			if (value->container) (*scopes.top())[Symbol::Instance] = value->container;

			Reference result = run(block->body, branch);
			scopes.pop();

			return Value::Copy(result);
		}
};