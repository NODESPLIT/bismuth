struct Source {
  string contents = "";
  int line = 0;
  int column = 0;
};

struct Token {
  static inline string Descriptor = "";

  static void Log(vector<Token>* tokens, string indent="", bool sectionFirst = false, bool sectionLast = false, bool parentLast = false) {
    int i = 0;
    int t = 0;
    for (auto &token : *tokens) {
      bool first = t == 0;
      bool last = t == tokens->size() - 1;
      bool content = token.children.size() > 0;

      if (!indent.empty() && first) {
        cout << indent.substr(0, indent.size() - 4) << "├─";
      } else {
        cout << indent;
      }

      cout << ( tokens->size() == 1 ? "─" : first ? ( indent.empty() ? "┌" : "┬" ) : last && !content ? "└" : "├" );

      cout << "─ ";
      cout << token.marker() << " ╴ ";
      cout << token.describe() << "  ← ";
      cout << token.code();
      cout << endl;
      
      if (token.is(Mark::End)) {
        if (!last) cout << indent << "│" << endl;
        i = -1;
      }

      if (content) cout << indent << "├─╮" << endl;

      int sections = 0;
      for (int s = 0; s < token.children.size(); s++) if (!token.children[s].empty()) sections++;

      int section = 0;
      for (int s = 0; s < token.children.size(); s++) {
        if (section != 0) cout << indent << "│ │ •" << endl;
        if (!token.children[s].empty()) {
          Log(&token.children[s], indent + "│ │ ", section == 0, s == sections - 1 && sections > 1, last);
          section++;
        }
      }

      if (sections > 0) cout << indent << ( last ? "└" : "├" ) << "─╯" << endl;
      
      i++;
      t++;
    }
  }

  static void Log(Token* token) {
    vector<Token> tokens = { *token };
    Log(&tokens);
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

  string describe() { return "<" + Names::Mark[mark] + ">"; };
  string code() { return "|" + regex_replace(contents, Utils::Flatten, " ") + "|"; }
  string marker() { return "[L" + std::to_string(source.line) + ":C" + std::to_string(source.column) + "]";  }

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