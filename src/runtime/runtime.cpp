typedef function<Reference(Runtime*, Reference, Reference)> Transform;
typedef unordered_map<Type, unordered_map<Type, Transform>> Transforms;

namespace Bindings { void Bind(Runtime* runtime); }

class Runtime {
	public:
		static void Log(Scope scope, int depth=0) {
			for (const auto& [ name, variable ] : *scope) {
				cout << variable->describe(depth, name + "[" + Names::Types[variable->type] + "]: ") << endl;
			}
		}

		bool returned = false;
		Reference result = Value::Empty();

		vector<string> words;
		vector<Reference> literals;

		Scope environment = make_shared<Table>();
		Scope global = make_shared<Table>();

		vector<Scope> origin;
		Scope current = global;

		void Push(Scope scope) {
			origin.push_back(current);
			current = scope ? scope : make_shared<Table>();
		}

		void Pop() {
			current = origin[origin.size() - 1];
			origin.pop_back();
		}

		void setup(Scope scope, string path="./", Reference in=nullptr) {
			(*scope)["@"] = Value::Locked(Type::Table);
			(*scope)["@"]->SET("path", Value::Lock(Value::Make(path)));
			(*scope)["@"]->SET("in", in ? Value::Lock(Value::Copy(in)) : Value::Empty());
		}

		Scope branched(Scope origin=nullptr) {
			Scope scope = make_shared<Table>();
			if (origin) for (const auto& [ name, variable ] : *origin) (*scope)[name] = variable;
			return scope;
		}

		Runtime() {
			Bindings::Bind(this);
			setup(global);
		}

		Runtime(string path, Reference in=nullptr) {
			Bindings::Bind(this);
			result = load(path, in, global);
		}

		Reference load(string path, Reference in=nullptr, Scope scope=nullptr) {
			path = Search::Path(path);

			if (!scope) scope = global;
			setup(scope, path, in);

			Push(scope);

			ifstream stream = ifstream(path);
			
			if (!stream.good()) return Value::Empty();
			string bismuth((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());

			if (Bismuth::Verbose) {
				cout << "\n\n--- Bismuth[" << path << "] ---" << endl;
				cout << endl << bismuth << endl;
			}

			Reference result = interpret(bismuth);

			Pop();
			return result;
		}

		Reference import(string path, Reference in=nullptr) {
			Scope subscope = branched();
			filesystem::path base((*current)["@"]->get("path")->as<string>()); base.remove_filename();
			return load(base / path, in, subscope);
		}

		Reference interpret(string bismuth) {
			vector<Token> tokens;
			Analyse(bismuth, &tokens);

			if (Bismuth::Verbose) {
				cout << "\n\n--- Tokens ---\n" << endl;
				Token::Log(&tokens);
			}

			return interpret(&tokens);
		}

		Reference interpret(vector<Token>* tokens) {
			vector<Node> tree;
			Parse(tokens, &tree);

			if (Bismuth::Verbose) {
				cout << "\n\n--- Tree ---\n" << endl;
				Node::Log(&tree);
			}

			return interpret(&tree);
		}

		Reference interpret(vector<Node>* tree) {
			vector<Instruction> instructions;
			for (int i = 0; i < tree->size(); i++) instructions.push_back(Instruction(this, &(*tree)[i]));

			if (Bismuth::Verbose) {
				cout << "\n\n--- Instructions ---\n" << endl;
				Instruction::Log(this, &instructions);
			}

			Reference result = resolve(instructions);
			
			if (Bismuth::Verbose) {
				cout << "\n\n--- Result ---\n" << endl;
				cout << result->describe() << endl << endl;
				cout << "\n--- Globals ---\n" << endl;
				Log(global);
				cout << endl << endl;
			}

			return result;
		}

		Reference resolve(vector<Instruction> instructions, bool returns=false) {
			Reference result = Value::Empty();
			
			for (int i = 0; i < instructions.size(); i++) {
				result = resolve(&instructions[i]);
				if (returned) { if (returns) returned = false; break; }
			}

			return result;
		}

		Reference resolve(Instruction* instruction) {
			switch(instruction->task) {
				case Task::Instruct: return Instruct(instruction);
				case Task::Literal: return Literal(instruction);
				case Task::Array: return LiteralArray(instruction);
				case Task::Table: return LiteralTable(instruction);
				case Task::Block: return LiteralBlock(instruction);
				case Task::Read: return Read(instruction);
				case Task::Inside: return Inside(instruction);
				case Task::Define: return Define(instruction);
				case Task::Operate: return Operate(instruction);
				case Task::Decide: return Decide(instruction);
				case Task::Call: return Call(instruction);
				case Task::Return: return Return(instruction);
				default: return Value::Empty();
			}
		}
		
		Reference call(Reference value, bool branch=true) { return call(value, {}, branch); }
		Reference call(Reference value, Array values, bool branch=true) {
			shared_ptr<Block> block = value->block;
			if (block->binding) return block->binding(values);

			Push(branch ? branched(block->context) : block->context);

			for (int i = 0; i < block->arguments.size(); i++) {
				if (i < values.size()) {
					(*current)[block->arguments[i]] = values[i];
				} else {
					Reference defaulting = resolve(&block->defaults[i]);
					(*current)[block->arguments[i]] = defaulting;
				}
			}

			Reference result = resolve(block->body, branch);
			Pop();

			return result;
		}

		Reference Instruct(Instruction* instruction);
		Reference Literal(Instruction* instruction);
		Reference LiteralArray(Instruction* instruction);
		Reference LiteralTable(Instruction* instruction);
		Reference LiteralBlock(Instruction* instruction);
		Reference Read(Instruction* instruction);
		Reference Inside(Instruction* instruction);
		Reference Define(Instruction* instruction);
		Reference Operate(Instruction* instruction);
		Reference Decide(Instruction* instruction);
		Reference Call(Instruction* instruction);
		Reference Return(Instruction* instruction);
};

namespace Operate { extern unordered_map<string, int> Reduced; }
namespace Names { extern vector<string> Operators; }

void Instruction::Log(Runtime* runtime, vector<Instruction>* instructions, int depth) {
	for (auto &instruction : *instructions) {
		cout << Utils::Indent(depth);
		cout << instruction.describe(runtime) << endl;
		if (instruction.task == Task::End) cout << endl;
		Log(runtime, &instruction.children, depth + 1);
	}
}

Instruction::Instruction(Runtime* runtime, Node* node) {
	task = node->task;
	
	if (task == Task::Literal || task == Task::Block) {
		runtime->literals.push_back(Value::Make(node));
		index = runtime->literals.size() - 1;
	} else if (task == Task::Read) {
		if (node->mark == Mark::Word) {
			runtime->words.push_back(node->contents);
			index = runtime->words.size() - 1;
		}
	} else if (task == Task::Operate) {
		if (Operate::Reduced.count(node->contents)) index = Operate::Reduced[node->contents];
	}

	for (int i = 0; i < node->children.size(); i++) {
		if (task == Task::Block) {
			if (i == 0) {
				for (int a = 0; a < node->children[i].children.size(); a += 2) {
					runtime->literals[index]->block->arguments.push_back(node->children[i].children[a].contents);
					runtime->literals[index]->block->defaults.push_back(Instruction(runtime, &(node->children[i].children[a + 1])));
				}
			} else {
				runtime->literals[index]->block->body.push_back(Instruction(runtime, &(node->children[i])));
			}
		} else {
			children.push_back(Instruction(runtime, &(node->children[i])));
		}
	}
}

string Instruction::describe(Runtime* runtime) {
	string description = "<" + Names::Task[task] + ">";
	if (task == Task::Literal && index > -1 && index < runtime->literals.size()) description += ": " + regex_replace(runtime->literals[index]->describe(), Utils::Flatten, " ");
	if (task == Task::Read && index > -1 && index < runtime->words.size()) description += ": " + runtime->words[index];
	if (task == Task::Operate && index > -1 && index < Names::Operators.size()) description += ": " + Names::Operators[index];
	return description;
}