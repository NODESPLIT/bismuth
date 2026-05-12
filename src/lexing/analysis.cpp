void Analyse(string bismuth, vector<Token>* target, bool listing) {
	string lastDescriptor = Token::Descriptor;

	Token* last = nullptr;
	Step step;

	Mark cued = Mark::None;
	int nesting = 0;

	string contents = "";

	bool complete = false;
	bool stay = false;
	bool single = false;

	char character;
	char before;

	int comment = 0;
	int lastComment = -1;
	string descriptor = "";

	int position = 0;

	while (position <= bismuth.size()) {
		before = character;
		character = position < bismuth.size() ? bismuth[position] : ' ';

		if (!Scanning::Gap && !comment) {
			comment = before == '/' && character == '/' ? 1 : before == '/' && character == '*' ? 2 : 0;
			// bug here when comment inside scope, breaks scope (because of nesting resetting), need to be able to add comments inside scope
			// maybe just make analysis state a struct and save its state before comments then return right after
			if (comment) {
				descriptor = "";
				contents = "";
				cued = Mark::None;
				nesting = 0;
				complete = false;
				stay = false;
				single = false;
			}
		}

		if ((comment == 1 && character == '\n') || (comment == 2 && before == '*' && character == '/')) {
			if (lastComment < 0) Bismuth::Header = Utils::Trim(Utils::Unindent(
				comment == 1
					? descriptor.substr(1, std::max(1, int(descriptor.size() - 1)))
					: descriptor.substr(1, std::max(1, int(descriptor.size() - 2)))
			));

			// could do this a lot better, header needs to be tied to the file thats imported really

			lastComment = comment;
			comment = false;

			if (comment == 2) position++;

			continue;
		}

		if (comment) {
			descriptor += character;
			position++;
			continue;
		}

		if (complete) {
			Token::Descriptor = descriptor.size() < 2 ? Token::Descriptor : Utils::Trim(
				lastComment == 1
					? descriptor.substr(1, std::max(1, int(descriptor.size() - 1)))
					: descriptor.substr(1, std::max(1, int(descriptor.size() - 2)))
			);

			Token token = Token(cued, stay ? contents.substr(0, contents.size() - 1) : contents);
			step = Scanners[cued].parse ? Scanners[cued].parse(&token, last, target, listing) : Step::Next;

			if (step == Step::Over && target->size() > 0) {
				(*target)[target->size() - 1] = token;
			} else {
				target->push_back(token);
			}
			
			if (!single && !stay) position++;

			last = &(*target)[target->size() - 1];

			cued = Mark::None;
			nesting = 0;

			contents = "";
			complete = false;
			stay = false;
			single = false;

			continue;
		}

		if (cued != Mark::None) {
			step = Scanners[cued].end(character, last, listing);
			
			if (step != Step::Skip) contents += character;
			if (step == Step::Nest) nesting++;

			if (nesting == 0) {
				if (step == Step::Stay) stay = true;
				if (step == Step::Stop || step == Step::Stay) { complete = true; continue; }
			} else if (step == Step::Stop || step == Step::Stay) {
				nesting--;
			}

			position++;
			continue;
		}

		if (character == ',' && !listing) {
			position++;
			continue;
		}

		if (character != '	' || character != ' ' || character != '\t') {
			for (const auto& [ type, scanner ] : Scanners) {
				step = scanner.cue(character, last, listing);
				
				if (step == Step::Next) continue;
				if (step == Step::Stop) { complete = true; single = true; }

				cued = type;
				contents += character;
				break;
			}

			Scanning::Gap = false;
		} else {
			Scanning::Gap = true;
		}

		position++;
	}

	Token::Descriptor = lastDescriptor;
}