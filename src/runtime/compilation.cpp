namespace Machine {
	void Log(Runtime* runtime, vector<Annotated>* instructions, int depth, string indent) {
		size_t digits = int(log10(instructions->size() - 1)) + 1;

		for (int i = 0; i < instructions->size(); i++) {
			Annotated* instruction = &(*instructions)[i];

			Operation wire = instruction->wire;
			int mode = instruction->mode;
			Task task = Machine::Tasks[wire];

			string line = std::to_string(i);
			line.insert(0, digits - line.size(), ' ');

			string description = line + " " + ( i == 0 ? "┬" : i == instructions->size() - 1 ? "┴" : "┼" ) + " <" + ( wire ? Names::Wiring[wire] : "JUMP" ) + ">";

			if (( task == Task::Value || task == Task::Array || task == Task::Table || task == Task::Block ) && mode > -1 && mode < runtime->literals.size()) description += ": " + regex_replace(runtime->literals[mode]->describe(), Utils::Flatten, " ");
			if (task == Task::Read || ( task == Task::Delete && mode > -1 && mode < runtime->words.size() )) description += ": " + runtime->words[mode];
			if (task == Task::Operate && Names::Operators.count(mode)) description += ": " + Names::Operators[mode];
			if (task == Task::Decide || task == Task::Jump) description += ": " + std::to_string(mode);

			if (instruction->children[0] > -1) {
				description += " <[ ";
				
				for (int i = 0; i < 2; i++) {
					if (instruction->children[i] > -1) {
						if (i != 0) description += ", ";
						description += std::to_string(instruction->children[i]);
					}
				}

				description += " ]>";
			}
			
			string prefix = indent;
			if (depth > -1) {
				prefix = prefix.substr(0, prefix.size() - 4);
				prefix += i == 0 ? "┌ " : i == instructions->size() - 1 ? "└ " : "├ ";
			}

			cout << prefix << description << endl;

			if (task == Task::Block) {
				string gap = " ";
				for (int d = 0; d < digits - 1; d++) gap += " ";
				cout << indent << gap << " ┊ " << endl;
				Log(runtime, &runtime->literals[instruction->mode]->block->body, depth + 1, indent + gap + " ┆ ");
				cout << indent << gap << " ┊ " << endl;
			}
		}
	}

	vector<int> Link(Runtime* runtime, vector<Node>* tree, vector<Annotated>* instructions, int offset = 0) {
		vector<int> children;

		string valueDescriptor = Value::Descriptor;
		string annotatedDescriptor = Annotated::Descriptor;

		for (int i = tree->size() - 1; i >= 0; i--) {
			Node* node = &(*tree)[i];
			Value::Descriptor = Annotated::Descriptor = node->descriptor;

			Task task = node->task;

			Operation wire = task == Task::End ? nullptr : Machine::Wiring[task];
			
			int mode = 0;
			int relation = -1;

			if (task == Task::Value || task == Task::Block) {
				runtime->literals.push_back(Value::Make(node));
				mode = runtime->literals.size() - 1;
			} else if (task == Task::Array || task == Task::Table) {
				runtime->relations.push_back({});
				relation = runtime->relations.size() - 1;
			} else if (task == Task::Read || task == Task::Delete) {
				if (node->mark == Mark::Word) {
					runtime->words.push_back(node->contents);
					mode = runtime->words.size() - 1;
				}
			} else if (task == Task::Operate) {
				if (Operate::Reduced.count(node->contents)) mode = Operate::Reduced[node->contents];
			}

			Annotated instruction = Annotated{ wire, mode };

			if (task == Task::Block) {
				vector<Node> head;
				
				int arg = 0;
				int spreaded = 0;

				for (int a = 0; a < node->children[0].children.size(); a += 2) {
					if (node->children[0].children[a].task == Task::Operate && node->children[0].children[a].contents == "*_") {
						head.push_back(
							Node(
								Task::Define,
								"=",
								{
									Node(Task::Read, Mark::Word, node->children[0].children[a].children[0].contents),
									Node(
										Task::Inside,
										Mark::Index,
										{
											Node(
												Task::Inside,
												Mark::Index,
												{
													Node(Task::Read, Mark::Word, Symbol::Context),
													Node(Task::Value, Mark::String, "args")
												}
											),
											Node(
												Task::Operate,
												Mark::Operator,
												"->",
												{
													Node(Task::Value, Mark::Number, std::to_string(a / 2)),
													Node(Task::Value, Mark::Number, std::to_string(spreaded = int((a / 2) - (node->children[0].children.size() / 2))))
												}
											)
										}
									)
								}
							)
						);

						runtime->literals[mode]->block->arguments.push_back("_");
					} else if (node->children[0].children[a].task == Task::Array || node->children[0].children[a].task == Task::Table) {
						Destructure(
							&node->children[0].children[a],
							&head,
							Node(
								Task::Inside,
								Mark::Index,
								{
									Node(
										Task::Inside,
										Mark::Index,
										{
											Node(Task::Read, Mark::Word, Symbol::Context),
											Node(Task::Value, Mark::String, "args")
										}
									),
									Node(Task::Value, Mark::String, std::to_string(arg))
								}
							)
						);

						runtime->literals[mode]->block->arguments.push_back("_");
					} else {
						if (spreaded < 0) {
							head.push_back(
								Node(
									Task::Define,
									"=",
									{
										Node(Task::Read, Mark::Word, node->children[0].children[a].contents),
										Node(
											Task::Inside,
											Mark::Index,
											{
												Node(
													Task::Inside,
													Mark::Index,
													{
														Node(Task::Read, Mark::Word, Symbol::Context),
														Node(Task::Value, Mark::String, "args")
													}
												),
												Node(Task::Value, Mark::Number, std::to_string(++spreaded))
											}
										)
									}
								)
							);

							runtime->literals[mode]->block->arguments.push_back("_");
						} else {
							runtime->literals[mode]->block->arguments.push_back(node->children[0].children[a].contents);
						}
					}

					vector<Node> defaulting = { node->children[0].children[a + 1] };
					runtime->literals[mode]->block->defaults.push_back({});
					Compile(runtime, &defaulting, &runtime->literals[mode]->block->defaults[arg]);

					arg++;
				}

				vector<Node> body = node->children; body.erase(body.begin());
				for (int h = head.size() - 1; h >= 0; h--) body.insert(body.begin(), head[h]);

				Compile(runtime, &body, &runtime->literals[mode]->block->body);
			}

			if (task == Task::Decide) {
				vector<Annotated> statement;
				for (int i = node->children.size() - 2; i >= 0; i -= 2) {
					int steps = statement.size();

					if (i < node->children.size() - 2) statement.push_back(Annotated{ Machine::Wiring[Task::Jump] });
					Link(runtime, &node->children[i + 1].children, &statement, instructions->size() + offset);

					steps = statement.size() - steps;

					Annotated decide = Annotated{ wire };
					decide.mode = steps;
					statement.push_back(decide);

					vector<Node> question = { node->children[i] };
					Link(runtime, &question, &statement, instructions->size() + offset);
				}

				int level = 1;
				for (int i = statement.size() - 1; i >= 0; i--) {
					if (!statement[i].wire) statement[i].mode = statement.size() - level;
					level++;
				}

				for (int i = 0; i < statement.size(); i++) instructions->push_back(statement[i]);
			} else {
				instructions->push_back(instruction);
				int self = instructions->size() - 1;
				children.push_back(self + offset);
				if (task != Task::Block) {
					vector<int> subchildren = Link(runtime, &node->children, instructions, offset);
					if ((task == Task::Array || task == Task::Table)) {
						for (int i = 0; i < subchildren.size(); i++) runtime->relations[relation].insert(runtime->relations[relation].begin(), subchildren[i]);
						(*instructions)[self].children[0] = relation;
					} else {
						for (int i = 0; i < subchildren.size(); i++) (*instructions)[self].children[i] = subchildren[i];
					}
				}
			}
		}

		Value::Descriptor = valueDescriptor;
		Annotated::Descriptor = annotatedDescriptor;

		return children;
	}

	void Compile(Runtime* runtime, vector<Node>* tree, vector<Annotated>* instructions) {
		vector<Annotated> reverse;
		Link(runtime, tree, &reverse);

		for (int i = reverse.size() - 1; i >= 0; i--) {
			Annotated real = reverse[i];

			if (real.wire == Machine::Wiring[Task::Array] || real.wire == Machine::Wiring[Task::Table]) {
				for (int r = 0; r < runtime->relations[real.children[0]].size(); r++) {
					runtime->relations[real.children[0]][r] = (reverse.size() - 1) - runtime->relations[real.children[0]][r];
				}
			} else {
				if (reverse[i].children[0] != -1 && reverse[i].children[1] != -1) {
					real.children[1] = (reverse.size() - 1) - reverse[i].children[0];
					real.children[0] = (reverse.size() - 1) - reverse[i].children[1];
				} else if (reverse[i].children[0] != -1) {
					real.children[0] = (reverse.size() - 1) - reverse[i].children[0];
				} else if (reverse[i].children[1] != -1) {
					real.children[1] = (reverse.size() - 1) - reverse[i].children[1];
				}
			}
			
			instructions->push_back(real);
		}
	}
}