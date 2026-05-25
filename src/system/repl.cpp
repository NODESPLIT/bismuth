struct Repl {
	Runtime runtime;

	Repl() {
		runtime = Runtime();

		linenoiseInstallWindowChangeHandler();
		printf("\x1b[1;31m%s\x1b[0m\n", Bismuth::Stamp.c_str());

		while (1) {
	    char* result = linenoise("\x1b[1;28m@>\x1b[0m ");

	    if (result == NULL) {
	    	break;
	    } else {
	    	string command = string(result);
	    	
	    	if (!command.compare("exit")) {
	    		break;
	    	} else {
	    		Reference result = runtime.interpret(command);
			    printf("\x1b[1;28m=>\x1b[0m %s\n", result->describe(0).c_str());
	    	}
	    }

	    linenoiseHistoryAdd(result);
	    free(result);
	  }

	  linenoiseHistoryFree();
	}
};