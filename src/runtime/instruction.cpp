class Runtime;
class Instruction {
	public:
		static void Log(Runtime* runtime, vector<Instruction>* instructions, int depth = 0);
		
		Instruction(Runtime* runtime, Node* node);
		string describe(Runtime* runtime);

		Task task; int index = -1;
		vector<Instruction> children;
};