#include <vector>
#include <utility>
#include "ir_instr.h"

IrInstr::IrInstr(IrInstrType type) {
  type_ = type;
}

IrInstr::IrInstr(IrInstrType type, std::string dest) {
  type_ = type;
  dest_ = dest;
  is_leader_ = false;
}

IrInstr::IrInstr(IrInstrType type,
		 std::pair<std::string, std::string> args,
		 std::string dest) {
  type_ = type;
  args_ = args;
  dest_ = dest;
  is_leader_ = false;
}

IrInstr::IrInstr(IrInstrType type,
		 std::string arg1,
		 std::string arg2,
		 std::string dest) {
  type_ = type;
  std::pair<std::string, std::string> arg_pair;
  arg_pair.first = arg1;
  arg_pair.second = arg2;
  args_ = arg_pair;
  dest_ = dest;
  is_leader_ = false;
}

bool IrInstr::IsJmp() {
  return (type_ == IrInstrType::JMP_INSTR ||
	  type_ == IrInstrType::JMPLT_INSTR ||
	  type_ == IrInstrType::JMPLTE_INSTR ||
	  type_ == IrInstrType::JMPGT_INSTR ||
	  type_ == IrInstrType::JMPGTE_INSTR ||
	  type_ == IrInstrType::JMPEQ_INSTR ||
	  type_ == IrInstrType::JMPNEQ_INSTR );
}
