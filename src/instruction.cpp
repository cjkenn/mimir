#include <vector>
#include <utility>
#include "instruction.h"

Instruction::Instruction(InstructionType type, std::string dest) {
  type_ = type;
  dest_ = dest;
}

Instruction::Instruction(InstructionType type,
			 std::pair<std::string, std::string> args,
			 std::string dest) {
  type_ = type;
  args_ = args;
  dest_ = dest;
}

Instruction::Instruction(InstructionType type,
			 std::string arg1,
			 std::string arg2,
			 std::string dest) {
  type_ = type;
  std::pair<std::string, std::string> arg_pair;
  arg_pair.first = arg1;
  arg_pair.second = arg2;
  args_ = arg_pair;
  dest_ = dest;
}

bool Instruction::IsJmp() {
  return (type_ == InstructionType::JMP_INSTR ||
    type_ == InstructionType::JMPLT_INSTR ||
    type_ == InstructionType::JMPGT_INSTR ||
	  type_ == InstructionType::JMPEQ_INSTR);
}
