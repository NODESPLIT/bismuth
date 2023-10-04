class Runtime;
class Instruction {
	public:
		static void Log(Runtime* runtime, vector<Instruction>* instructions, int depth = 0);
		
		Instruction(Runtime* runtime, Node* node);
		string describe(Runtime* runtime);

		Reference (*wire)(Instruction*);
		Reference resolve();

		int index = -1;
		vector<Instruction> children;
};