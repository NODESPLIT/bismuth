Token* Read(vector<Token>* tokens, int at) { if (at >= tokens->size() || at < 0) return nullptr; return &(*tokens)[at]; }

void Parse(vector<Token>* tokens, vector<Node>* nodes);

void Branch(vector<Token>* tokens, vector<Node>* target, int position=0, Node* leftNode=nullptr);
void Branch(Token* token, vector<Node>* target, int position=0, Node* leftNode=nullptr);

void BranchList(vector<Token>* tokens, vector<Node>* target) {
	vector<Token> element;
	for (int i = 0; i <= tokens->size(); i++) {
		if (i == tokens->size() || (*tokens)[i].is(Mark::Comma) || (*tokens)[i].is(Mark::End)) {
			Branch(&element, target);
			element.clear();
		} else {
			element.push_back((*tokens)[i]);
		}
	}
}

void Branch(vector<Token>* tokens, vector<Node>* target, int position, Node* leftNode) {
	Token* left = Read(tokens, position); if (!left || left->is(Mark::End)) return;
	Token* middle = Read(tokens, position + 1);

	if (left->is(Mark::Word, "return")) {
		Node node = Node{ Task::Return };
		Branch(tokens, &node.children, position + 1);
		target->push_back(node);
		return;
	}

	if (!middle) {
		if (!leftNode) {
			if (left->is(Mark::Brackets)) {
				Branch(&left->children[0], target);
			} else if (left->is(vector<Mark>{ Mark::Number, Mark::String })) {
				target->push_back(Node{ Task::Value, left });
			} else if (left->is(Mark::Array)) {
				Node node = Node{ Task::Array, left };
				BranchList(&left->children[0], &node.children);
				target->push_back(node);
			} else if (left->is(Mark::Table)) {
				Node node = Node{ Task::Table, left };
				vector<Token> part;

				bool defining = false;
				for (int i = 0; i <= left->children[0].size(); i++) {
					if (i == left->children[0].size() || ( defining ? left->children[0][i].is(Mark::Comma) : left->children[0][i].is(Mark::Operator, ":") ) || left->children[0][i].is(Mark::End)) {
						if (defining) {
							Branch(&part, &node.children);
						} else if (part.size() > 0) {
							vector<Token> name = { Token{ Mark::String, part[part.size() - 1].contents } };
							Branch(&name, &node.children);
						}
						part.clear(); defining = !defining;
					} else {
						part.push_back(left->children[0][i]);
					}
				}

				target->push_back(node);
			} else if (left->is(Mark::Block)) {
				Node node = Node{ Task::Block, left };
				Node arguments = Node{ Task::Value };

				Token argument;
				vector<Token> defaults;
				bool defaulting = false;
				
				for (int i = 0; i <= left->children[0].size(); i++) {
					if (i == left->children[0].size() || left->children[0][i].is(Mark::Comma)) {
						if (argument.isnt(Mark::End)) {
							Branch(&argument, &arguments.children);
							if (defaults.empty()) defaults.push_back(Token(Mark::Word, "void"));
							Branch(&defaults, &arguments.children);
						}
						defaults.clear(); defaulting = false;
						argument = Token(Mark::End);
					} else {
						if (defaulting) {
							defaults.push_back(left->children[0][i]);
						} else {
							if (left->children[0][i].is(Mark::Operator, "=")) {
								defaulting = true;
							} else {
								argument = left->children[0][i];
							}
						}
					}
				}

				node.children.push_back(arguments);
				Parse(&left->children[1], &node.children);

				target->push_back(node);
			} else if (left->is(Mark::Index)) {
				Node node = Node{ Task::Inside };

				Branch(&left->children[0], &node.children);
				Branch(&left->children[1], &node.children);

				for (int i = 2; i < left->children.size(); i++) {
					Node origin = Node(node);
					node = Node{ Task::Inside, left, { Node(node) } };
					Branch(&left->children[i], &node.children);
				}

				target->push_back(node);
			} else if (left->is(Mark::Word)) {
				target->push_back(Symbols.count(left->contents) ? Node(Symbols[left->contents]) : Node{ Task::Read, left });
			} else if (left->is(Mark::If)) {
				Node node = Node{ Task::Decide };

				for (int i = 0; i < left->children.size(); i += 2) {
					vector<Token> question = left->children[i];
					if (question.empty()) question = { Token(Mark::Word, "true") };

					vector<Token> answer = left->children[i + 1];
					Parse(&question, &node.children);

					Node resolution = Node{ Task::Array, left->contents };
					Parse(&answer, &resolution.children);
					
					node.children.push_back(resolution);
				}

				target->push_back(node);
			} else if (left->is(Mark::Call)) {
				Node node = Node{ Task::Call };

				Branch(&left->children[0], &node.children);

				Node arguments = Node{ Task::Array, Mark::Array };
				BranchList(&left->children[1], &arguments.children);
				node.children.push_back(arguments);

				target->push_back(node);
			}
		}

		return;
	}

	if (left->is(Mark::Operator) && middle) {
		Node node = Node{ Task::Operate, left->contents + "_" };
		Branch(middle, &node.children);

		if (!Read(tokens, position + 2)) {
			target->push_back(node);
		} else {
			Branch(tokens, target, position + 1, &node);
		}

		return;
	}

	if (middle->is(Mark::Operator)) {
		Operator rightOperator = Operators[middle->contents];
		Node node = Node{ rightOperator.effect == Effect::Assignment ? Task::Define : Task::Operate, middle->contents };
		bool merges = false;

		if (node.task == Task::Define || rightOperator.orientation == Orientation::RightToLeft) {
			if (!rightOperator.suboperator.empty() && Operators.count(rightOperator.suboperator)) {
				if (rightOperator.pairing == Pairing::Unary) {
					Branch(Read(tokens, position), &node.children);
					Token* next = Read(tokens, position + 2);
					node.contents = "_" + node.contents;

					if (next && next->is(Mark::Operator)) {
						Branch(tokens, target, position + 1, &node);
						return;
					}
				} else {
					Node suboperation = Node{ Task::Operate, rightOperator.suboperator };
					Branch(Read(tokens, position), &suboperation.children);
					Branch(tokens, &suboperation.children, position + 2);
					Branch(Read(tokens, position), &node.children);
					node.children.push_back(suboperation);
				}
			} else {
				Branch(Read(tokens, position), &node.children);
	      Branch(tokens, &node.children, position + 2);
			}

      target->push_back(node);
      return;
    }

		if (leftNode && rightOperator.priority < Operators[leftNode->contents].priority) {
			Node right = Node(leftNode->children[1]);
			node.children.push_back(right);
			merges = true;
		} else {
			if (leftNode) {
				node.children.push_back(Node(*leftNode));
			} else {
				Branch(left, &node.children, 0);
			}
		}

		Branch(Read(tokens, position + 2), &node.children);
		if (merges) node = Node{ leftNode->task, leftNode->mark, leftNode->contents, { Node(leftNode->children[0]), Node(node) } };
		Branch(tokens, target, position + 2, &node);

		if (!Read(tokens, position + 3)) target->push_back(node);
	}
}

void Branch(Token* token, vector<Node>* target, int position, Node* leftNode) {
	vector<Token> tokens = { *token };
	Branch(&tokens, target, position, leftNode);
}

void Parse(vector<Token>* tokens, vector<Node>* nodes) {
	vector<Token> line;

	for (int i = 0; i <= tokens->size(); i++) {
		if (i == tokens->size() || (*tokens)[i].is(Mark::End)) {
			if (line.size() > 0) {
				Branch(&line, nodes);
				line.clear();
			}
		} else {
			line.push_back((*tokens)[i]);
		}
	}
}