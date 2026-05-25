struct Node {
	static inline string Descriptor = "";

	static void Log(vector<Node>* nodes, int depth = 0) {
		for (auto &node : *nodes) {
			cout << Utils::Indent(depth);
			cout << node.describe() << endl;
			if (node.task == Task::End) cout << endl;
			Log(&node.children, depth + 1);
		}
	}

	Node(){}
	Node(Node* origin) { swap(origin); }
	Node(Task task) { this->task = task; }
	Node(Task task, vector<Node> children) { this->task = task; this->children = children; }

	Node(Task task, Token* token) { this->task = task; this->mark = token->mark; this->contents = token->contents; this->descriptor = token->descriptor; }
	Node(Task task, Token* token, vector<Node> children) { this->task = task; this->mark = token->mark; this->contents = token->contents; this->children = children; this->descriptor = token->descriptor; }

	Node(Task task, Mark mark) { this->task = task; this->mark = mark; }
	Node(Task task, Mark mark, vector<Node> children) { this->task = task; this->mark = mark; this->children = children; }
	Node(Task task, Mark mark, string contents) { this->task = task; this->mark = mark; this->contents = contents; }
	Node(Task task, Mark mark, string contents, vector<Node> children) { this->task = task; this->mark = mark; this->contents = contents; this->children = children; }

	Node(Task task, string contents) { this->task = task; this->contents = contents; }
	Node(Task task, string contents, vector<Node> children) { this->task = task; this->contents = contents; this->children = children; }

	string describe() {
		string description =  "<" + Names::Task[task] + ">";
		if (mark != Mark::None) description += "[" + Names::Mark[mark] + "]";
		if (!contents.empty()) description += ": " + regex_replace(contents, Utils::Flatten, " ");
		// if (!descriptor.empty()) description += " - \"" + descriptor + "\"";
		return description;
	}

	void swap(Node* with) {
		task = with->task;
		mark = with->mark;
		descriptor = with->descriptor;
		contents = with->contents;
		children = {};
		for (int i = 0; i < with->children.size(); i++) children.push_back(Node(&with->children[i]));
	}

	Node detail(string descriptor) {
		Node change = Node(this);
		change.descriptor = descriptor;
		return change;
	}

	vector<Node> children;

	Task task;
	Mark mark = Mark::None;

	string descriptor = Descriptor;
	string contents;

	Source source;
};