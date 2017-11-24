#include <vector>
#include <utility>
#include "ir_instr.h"
#include "pretty_printer.h"

using namespace mimir;

IrInstr::IrInstr(IrInstrType type) {
  type_ = type;
  redundant_ = false;
  constant_op_ = false;
  constant_val_ = 0;
}

IrInstr::IrInstr(IrInstrType type,
		 std::pair<std::string, std::string> args,
		 std::string dest) {
  type_ = type;
  args_ = args;
  dest_ = dest;
  is_leader_ = false;
  redundant_ = false;
  constant_op_ = false;
  constant_val_ = 0;
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
  redundant_ = false;
  constant_op_ = false;
  constant_val_ = 0;
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

bool IrInstr::IsBinOp() {
  return (type_ == IrInstrType::ADD_INSTR ||
	  type_ == IrInstrType::SUB_INSTR ||
	  type_ == IrInstrType::MUL_INSTR ||
	  type_ == IrInstrType::DIV_INSTR ||
	  type_ == IrInstrType::MOD_INSTR);
}

bool IrInstr::IsFunc() {
  return (type_ == IrInstrType::FUNC_ENTER_INSTR);
}

bool IrInstr::IsCall() {
  return (type_ == IrInstrType::FUNC_CALL_INSTR);
}

const std::string IrInstr::GetTypeAsStr() {
  return PrettyPrinter::ir_map[type_];
}

bool IrInstr::IsCommutative() {
  return (type_ == IrInstrType::ADD_INSTR ||
	  type_ == IrInstrType::MUL_INSTR);
}

void IrInstr::ConvertConstant() {
  type_ = IrInstrType::MV_INSTR;
  args_.first = std::to_string(constant_val_);
}
