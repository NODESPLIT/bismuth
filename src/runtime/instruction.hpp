struct Cursor {
	int position = 0;
	int jump = 0;
	int go = -1;
};

typedef Reference (*Operation)(Reference[], Instruction*, Reference, Cursor*);

struct Instruction {
	Operation wire;
	int mode; int children[2] = { -1, -1 };
};