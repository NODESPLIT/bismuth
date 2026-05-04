class Runtime;
namespace Bindings { void Bind(Runtime* runtime); }
namespace Operate { extern unordered_map<string, int> Reduced; }

#include "instruction.hpp"

namespace Machine {
	Reference VALUE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference ARRAY(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference TABLE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference BLOCK(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference READ(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference INSIDE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference DEFINE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference DELETE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference OPERATE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference DECIDE(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference CALL(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);
	Reference RETURN(Reference state[], Instruction* instruction, Reference last, Cursor* cursor);

	void Log(Runtime* runtime, vector<Annotated>* instructions);
	void Compile(Runtime* runtime, vector<Node>* tree, vector<Annotated>* instructions);

	extern unordered_map<Task, Operation> Wiring;
	extern unordered_map<Operation, Task> Tasks;
}

namespace Names {
	extern unordered_map<Operation, string> Wiring;
	extern unordered_map<int, string> Operators;
}