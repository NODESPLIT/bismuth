namespace Log {
	string Indent(int depth) {
		string indentation = "";
		for (int i = 0; i < depth; i++) indentation += "  ";
		return indentation;
	}
};