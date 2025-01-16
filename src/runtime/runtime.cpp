Runtime* Instance;

class Runtime {
	public:
		#include "interpretation.cpp"

		vector<string> words;
		vector<Reference> literals;
		vector<vector<int>> relations;

		stack<Scope> scopes;
		Scope environment = Scope(new Table());
		Scope global = Scope(new Table());

		Reference result;
		bool returned = false;

		Runtime() { Bindings::Bind(this); init(global); }
		Runtime(string path, Reference in=nullptr) { Bindings::Bind(this); result = load(path, in, global); }

		void init(Scope scope, string path="./", Reference in=nullptr) {
			(*scope)["@path"] = Value::Lock(Value::Make(path));
			(*scope)["@in"] = in ? Value::Lock(Value::Copy(in)) : Value::Empty();
			scopes.push(scope);
		}

		Scope branched(Scope origin=nullptr) {
			if (origin) return boost::make_shared<Table>(*origin);
			return Scope(new Table());
		}

		Reference call(Reference value, bool branch=true) { return call(value, {}, branch); }
		Reference call(Reference value, Array values, bool branch=true) {
			shared_ptr<Block> block = value->block;
			if (block->binding) return block->binding(values);

			scopes.push(branch ? branched(block->context) : block->context);

			for (int i = 0; i < block->arguments.size(); i++) {
				if (i < values.size()) {
					(*scopes.top())[block->arguments[i]] = Value::Copy(values[i]);
				} else {
					Reference defaulting = run(block->defaults[i]);
					(*scopes.top())[block->arguments[i]] = defaulting;
				}
			}

			Reference result = run(block->body, branch);
			scopes.pop();

			return Value::Copy(result);
		}

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
};