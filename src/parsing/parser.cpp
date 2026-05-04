int Destructured = 0;

void Destructure(Node* parent, vector<Node>* output, Node source, vector<string> path = {}) {
	Node::Descriptor = parent->descriptor;
	
	if (parent->task == Task::Table) {
		for (int i = 0; i < parent->children.size(); i += 2) {
			Node key = parent->children[i];
			Node value = parent->children[i + 1];

			if (value.task == Task::Array || value.task == Task::Table) {
				vector<string> subpath = path;
				subpath.push_back(key.contents);
				Destructure(&value, output, source, subpath);
			} else {
				bool spreading = key.task == Task::Operate && key.contents == "*_";
				Node pathing;

				for (int p = 0; p <= path.size(); p++) {
					if (spreading) {
						if (path.size() == 0) pathing = source;
						if (p == path.size()) break;
					}

					pathing = Node(
						Task::Inside,
						Mark::Index,
						{
							p == 0 ? source : pathing,
							Node(Task::Value, Mark::String, p == path.size() ? key.contents : path[p])
						}
					);
				}

				if (spreading) {
					Node keys = Node(Task::Array, Mark::Array);

					for (int k = 0; k < i; k += 2) if (parent->children[k + 1].mark == Mark::Word) keys.children.push_back(
						Node(
							Task::Value,
							Mark::String,
							parent->children[k].contents
						)
					);

					output->push_back(
						Node(
							Task::Define,
							"=",
							{
								Node(Task::Read, Mark::Word, key.children[0].contents),
								Node(
									Task::Operate,
									Mark::Operator,
									"-:",
									{
										pathing,
										keys
									}
								)
							}
						)
					);
				} else if (value.mark == Mark::Word) {
					output->push_back(
						Node(
							Task::Define,
							"=",
							{
								Node(Task::Read, Mark::Word, key.contents),
								pathing
							}
						)
					);
				}
			}
		}
	} else {
		int spreaded = 0;

		for (int i = 0; i < parent->children.size(); i++) {
			Node node = parent->children[i];
			bool spreading = node.task == Task::Operate && node.contents == "*_";
			
			if (spreading || node.mark == Mark::Word) {
				Node pathing;

				for (int p = 0; p <= path.size(); p++) {
					pathing = Node(
						Task::Inside,
						Mark::Index,
						{
							p == 0 ? source : pathing,
							spreading && p == path.size()
								? Node(
									Task::Operate,
									"->",
									{
										Node(Task::Value, Mark::Number, std::to_string(i)),
										Node(Task::Value, Mark::Number, std::to_string(spreaded = int(i - parent->children.size())))
									}
								)
								: Node(
									Task::Value, Mark::String,
									spreaded == 0 ? ( p == path.size() ? std::to_string(i) : path[p] ) : std::to_string(++spreaded)
								)
						}
					);
				}

				output->push_back(
					Node(
						Task::Define,
						"=",
						{
							Node(Task::Read, Mark::Word, spreading ? node.children[0].contents : node.contents),
							pathing
						}
					)
				);
			} else if (node.task == Task::Array || node.task == Task::Table) {
				vector<string> subpath = path;
				subpath.push_back(std::to_string(i));
				Destructure(&node, output, source, subpath);
			}
		}
	}
}

Node Spread(Node origin) {
	Node::Descriptor = origin.descriptor;
	Node output = Node(origin.task, origin.mark);

	if (origin.task == Task::Table) {
		vector<Node> parts;

		for (int i = 0; i < origin.children.size(); i += 2) {
			Node key = origin.children[i];
			Node value = origin.children[i + 1];

			if (key.task == Task::Operate) {
				Node addition = parts.empty() ? key.children[0] : Node(
					Task::Operate,
					"+",
					{
						Node(Task::Table, Mark::Table, parts),
						key.children[0]
					}
				);

				output = Node(
					Task::Operate,
					"+",
					{
						output,
						addition
					}
				);
				
				parts.clear();
			} else {
				parts.push_back(key);
				parts.push_back(value);
			}

			if (i == origin.children.size() - 2) {
				if (i == 0 && origin.children.size() == 2) {
					output = Node(
						Task::Operate,
						"+",
						{
							Node(Task::Table, Mark::Table, parts),
							output
						}
					);
				} else {
					output = Node(
						Task::Operate,
						"+",
						{
							output,
							Node(Task::Table, Mark::Table, parts)
						}
					);
				}
			}
		}
	} else if (origin.task == Task::Array) {
		vector<Node> parts;

		for (int i = 0; i < origin.children.size(); i++) {
			Node value = origin.children[i];

			if (value.task == Task::Operate) {
				Node addition = parts.empty() ? value.children[0] : Node(
					Task::Operate,
					"+",
					{
						Node(Task::Array, Mark::Array, parts),
						value.children[0]
					}
				);

				output = Node(
					Task::Operate,
					"+",
					{
						output,
						addition
					}
				);
				
				parts.clear();
			} else {
				parts.push_back(value);
			}

			if (i == origin.children.size() - 1) {
				if (i == 0 && origin.children.size() == 1) {
					output = Node(
						Task::Operate,
						"+",
						{
							Node(Task::Array, Mark::Array, parts),
							output
						}
					);
				} else {
					output = Node(
						Task::Operate,
						"+",
						{
							output,
							Node(Task::Array, Mark::Array, parts)
						}
					);
				}
			}
		}
	}

	return output;
}

Token* Read(vector<Token>* tokens, int at) { if (at >= tokens->size() || at < 0) return nullptr; return &(*tokens)[at]; }

void Parse(vector<Token>* tokens, vector<Node>* nodes);

void Branch(vector<Token>* tokens, vector<Node>* target, int position=0, Node* leftNode=nullptr, bool nospread=false);
void Branch(Token* token, vector<Node>* target, int position=0, Node* leftNode=nullptr, bool nospread=false);

bool BranchList(vector<Token>* tokens, vector<Node>* target, bool nospread=false) {
	vector<Token> element;
	bool spread = false;

	for (int i = 0; i <= tokens->size(); i++) {
		if (element.size() == 0 && i < tokens->size() && (*tokens)[i].is(Mark::Operator, "*")) spread = true;

		if (i == tokens->size() || (*tokens)[i].is(Mark::Comma) || (*tokens)[i].is(Mark::End)) {
			Branch(&element, target, 0, nullptr, nospread);
			element.clear();
		} else {
			element.push_back((*tokens)[i]);
		}
	}

	return spread;
}

void Branch(vector<Token>* tokens, vector<Node>* target, int position, Node* leftNode, bool nospread) {
	Token* left = Read(tokens, position); if (!left || left->is(Mark::End)) return;
	Token* middle = Read(tokens, position + 1);

	Token::Descriptor = left->descriptor;
	Node::Descriptor = left->descriptor;

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
				bool spread = BranchList(&left->children[0], &node.children);
				target->push_back(spread && !nospread ? Spread(node) : node);
			} else if (left->is(Mark::Table)) {
				Node node = Node{ Task::Table, left };
				vector<Token> part;

				bool defining = false;
				bool spreading = false;
				bool spread = false;

				for (int i = 0; i <= left->children[0].size(); i++) {
					if (i > 0 && !defining && left->children[0][i - 1].is(Mark::Operator, "*")) {
						part.push_back(left->children[0][i]);
						defining = true;
						spreading = true;
						spread = true;
						continue;
					}

					if (i == left->children[0].size() || ( !defining && left->children[0][i].is(Mark::Operator, ":") ) || left->children[0][i].is(Mark::Comma) || left->children[0][i].is(Mark::End)) {
						if (defining) {
							Branch(&part, &node.children, 0, nullptr, true);
							if (spreading) node.children.push_back(Node(Task::Value, Mark::Word, "void"));
							defining = false;
							spreading = false;
						} else if (part.size() > 0) {
							vector<Token> name = { Token( Mark::String, part[part.size() - 1].contents ) };
							
							Branch(&name, &node.children, 0, nullptr, true);
							if (i == left->children[0].size() || left->children[0][i].is(Mark::Comma) || left->children[0][i].is(Mark::End)) {
								vector<Token> destructure = { Token( Mark::Word, part[part.size() - 1].contents ) };
								Branch(&destructure, &node.children, 0, nullptr, true);
							}
							
							defining = i == left->children[0].size() || ( !left->children[0][i].is(Mark::Comma) && !left->children[0][i].is(Mark::End) );
						}
						part.clear();
					} else {
						part.push_back(left->children[0][i]);
					}
				}

				target->push_back(spread && !nospread ? Spread(node) : node);
			} else if (left->is(Mark::Block)) {
				Node node = Node{ Task::Block, left };
				Node arguments = Node{ Task::Value };

				Token argument;
				vector<Token> defaults;
				bool defaulting = false;
				bool spreading = false;
				
				for (int i = 0; i <= left->children[0].size(); i++) {
					if (i == left->children[0].size() || left->children[0][i].is(Mark::Comma)) {
						if (argument.isnt(Mark::End)) {
							if (spreading) {
								arguments.children.push_back(Node(Task::Operate, Mark::Operator, "*_", { Node(Task::Read, Mark::Word, argument.contents) }));
							} else {
								Branch(&argument, &arguments.children, 0, nullptr, true);
							}

							if (defaults.empty() || spreading) {
								Token part = Token(Mark::Word, "void");
								defaults.push_back(part);
							}

							Branch(&defaults, &arguments.children);
						}

						defaults.clear(); defaulting = false; spreading = false;
						argument = Token(Mark::End);
					} else {
						if (defaulting) {
							defaults.push_back(left->children[0][i]);
						} else {
							if (left->children[0][i].is(Mark::Operator, "=")) {
								defaulting = true;
							} else if (left->children[0][i].is(Mark::Operator, "*") || left->children[0][i].is(Mark::Operator, "*_")) {
								spreading = true;
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

				bool spread = BranchList(&left->children[1], &arguments.children);
				node.children.push_back(spread && !nospread ? Spread(arguments) : arguments);

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
				Branch(Read(tokens, position), &node.children, 0, nullptr, true);
	      Branch(tokens, &node.children, position + 2);
			}

			if (node.task == Task::Define && ( node.children[0].task == Task::Array || node.children[0].task == Task::Table )) {
				Node source = node.children[1];
				
				if (source.task == Task::Array || source.task == Task::Table) {
					string alias = "___DESTRUCTURE___";
					
					target->push_back(
						Node(
							Task::Define,
							"=",
							{
								Node(Task::Read, Mark::Word, alias),
								source
							}
						)
					);

					source = Node(Task::Read, Mark::Word, alias);
		      Destructure(&node.children[0], target, source);

		      target->push_back(
						Node(
							Task::Delete,
							Mark::Word,
							alias
						)
					);
				} else {
		      Destructure(&node.children[0], target, source);
				}

	      return;
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

void Branch(Token* token, vector<Node>* target, int position, Node* leftNode, bool nospread) {
	vector<Token> tokens = { *token };
	Branch(&tokens, target, position, leftNode, nospread);
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