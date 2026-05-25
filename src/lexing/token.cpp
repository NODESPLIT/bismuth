struct Source {
  string contents = "";
  int line = 0;
  int column = 0;
};

struct Token {
  static inline string Descriptor = "";

  static void Log(vector<Token>* tokens, int depth = 0) {
    for (auto &token : *tokens) {
      cout << Utils::Indent(depth);
      cout << token.describe() << endl;
      if (token.is(Mark::End)) cout << endl;
      for (int s = 0; s < token.children.size(); s++){
        if (s != 0) { cout << Utils::Indent(depth + 1) << "--" << endl; }
        Log(&token.children[s], depth + 1);
      }
    }
  }

  static void Log(Token* token, int depth = 0) {
    vector<Token> tokens = { *token };
    Log(&tokens, depth);
  }

  bool contains(string contents) { return contents.compare(this->contents) == 0; }
  bool contains(vector<string> values) { for (auto &contents : values) if (contains(contents)) return true; return false; }

  bool is(Mark mark) { return mark == this->mark; }
  bool is(vector<Mark> marks) { for (auto &mark : marks) if (is(mark)) return true; return false; }
  bool is(Mark mark, string contents) { return is(mark) && contains(contents); }
  bool is(Mark mark, vector<string> contents) { return is(mark) && contains(contents); }

  bool isnt(Mark mark) { return !is(mark); }
  bool isnt(vector<Mark> marks) { return !is(marks); }
  bool isnt(Mark mark, string contents) { return !is(mark, contents); }
  bool isnt(Mark mark, vector<string> contents) { return !is(mark, contents); }

  void swap(Token* with) {
    mark = with->mark;
    descriptor = with->descriptor;
    contents = with->contents;
    source = with->source;
    children = {};
    for (int i = 0; i < with->children.size(); i++) {
      children[i] = {};
      for (int t = 0; t < with->children[i].size(); t++) children[i].push_back(Token(&children[i][t]));
    }
  }

  string describe() { return "<" + Names::Mark[mark] + "> [L" + std::to_string(source.line) + "|C" + std::to_string(source.column) + "] : |" + regex_replace(contents, Utils::Flatten, " ") + "|";  };

  Token detail(string descriptor) {
    Token change = Token(this);
    change.descriptor = descriptor;
    return change;
  }

  void relocate(Token* other) {
    source.line = other->source.line;
    source.column = other->source.column;
  }

  vector<vector<Token>> children;
  
  Mark mark = Mark::End;
  string descriptor = Descriptor;
  string contents = "";
  Source source;

  Token(){}
  Token(Token* origin) {
    mark = origin->mark;
    descriptor = origin->descriptor;
    contents = origin->contents;
    children = origin->children;
    source.contents = contents;
  }

  Token(Mark mark, Source source={}) { this->mark = mark; this->source = source; }
  
  Token(Mark mark, string contents, Source source={}) {
    this->mark = mark;
    this->contents = contents;
    source.contents = contents;
    this->source = source;
  }

  Token(Mark mark, string contents, vector<vector<Token>> children, Source source={}) {
    this->mark = mark;
    this->contents = contents;
    source.contents = contents;
    this->children = children;
    this->source = source;
  }
};