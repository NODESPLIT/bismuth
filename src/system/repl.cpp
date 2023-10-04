class Repl {
	public:
		static constexpr char* Prompt = "$>";
		
		Runtime runtime;
		WINDOW* window;

		string command;
		string current = "";
		int cursor = 0;
		int back = 0;

		vector<string> history = {};
		bool running = true;

		Repl() {
			runtime = Runtime();
			
			initscr();
			setlocale(LC_ALL, "");
			cbreak();
			noecho();
		  
			int height, width;
		  getmaxyx(stdscr, height, width);
		  window = newwin(height, width, 0, 0);

		  keypad(window, true);
		  nodelay(window, true);
		  scrollok(window, true);
		  wclrtobot(window);

		  print(Stamp);
		  while(running) prompt();

		  endwin();
		}

		void execute() {
			if (command.compare("exit") == 0) { running = false; return; }
			print(""); Reference result = runtime.interpret(command);
			int x, y; getyx(window, y, x); wmove(window, y, 0);
			print("=> " + result->describe(0));
		}

		void line() {
			int x, y; getyx(window, y, x); wmove(window, y, 0); wclrtoeol(window);
			wprintw(window, "%s %s", Prompt, command.c_str());
			wmove(window, y, cursor + 3);
			wrefresh(window);
		}

		void move(int to) {
			cursor = std::max(0, std::min(int(command.size()), cursor + to));
		}

		void fresh() { wprintw(window, "\n"); wrefresh(window); }
		void print(string line) { wprintw(window, line.c_str()); fresh(); } 

		void prompt() {
			fresh();

			command = "";
			int character;

		  line();

			bool proceed = false;
		  while (!proceed) {
		  	character = wgetch(window);

		  	switch (character) {
			    case KEY_BACKSPACE:
			      if (cursor > 0 && cursor <= command.size()) {
			      	back = 0;
			      	command.erase(cursor - 1, 1);
				      cursor--;
				    }
			      break;
			    case KEY_LEFT:
			      move(-1);
			      break;
			    case KEY_RIGHT:
			      move(1);
			      break;
			    case KEY_UP:
			    	if (back == 0) current = command;
			    	if (back < history.size()) {
			    		back++;
				    	command = history[history.size() - back];
				    	cursor = command.size();
				    }
			      break;
			    case KEY_DOWN:
			    	if (back > 1) {
			    		back--;
				    	command = history[history.size() - back];
				    	cursor = command.size();
				    } else if (back == 1) {
				    	back--;
				    	command = current;
				    	cursor = command.size();
				    }
			      break;
			    case 10:
			      proceed = true;
		        break;
			    default:
			      if (iscntrl(character) || character != -1) {
			      	back = 0;
			      	if (cursor == command.size()) {
			      		command += character;
			      	} else {
				      	command.insert(cursor, string{ static_cast<char>(character) });
				      }
			      	cursor++;
			      } else {
			      	beep();
			      }

		        break;
		    }

		    line();
		  }

			int x, y; getyx(window, y, x);
			wmove(window, y, command.size() + 3);

			if (!command.empty()) {
				execute();
				history.push_back(command);
			}

			command = "";
			cursor = 0;
		}
};