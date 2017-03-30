#include "x86_instr.h"

X86Instr::X86Instr(X86InstrType type) {
  type_ = type;
}

X86Instr::X86Instr(X86InstrType type, std::string label) {
  type_ = type;
  label_ = label;
}

std::string X86Instr::Serialize() {
  std::string op = MapTypeToStr();

  if (!args_.first.empty()) {
    op += (" " + args_.first);
  }

  if (!args_.second.empty()) {
    op += (", " + args_.second);
  }

  return op;
}

std::string X86Instr::MapTypeToStr() {
  switch(type_) {
  case X86InstrType::MOV_X86:
    return "mov";
  case X86InstrType::ADD_X86:
    return "add";
  case X86InstrType::SUB_X86:
    return "sub";
  case X86InstrType::MUL_X86:
    return "mul";
  case X86InstrType::DIV_X86:
    return "div";
  case X86InstrType::CMP_X86:
    return "cmp";
  case X86InstrType::JMP_X86:
    return "jmp";
  case X86InstrType::JE_X86:
    return "je";
  case X86InstrType::JNE_X86:
    return "jne";
  case X86InstrType::JL_X86:
    return "jl";
  case X86InstrType::JLE_X86:
    return "jle";
  case X86InstrType::JG_X86:
    return "jg";
  case X86InstrType::JGE_X86:
    return "jge";
  case X86InstrType::PUSH_X86:
    return "push";
  case X86InstrType::POP_X86:
    return "pop";
  case X86InstrType::CALL_X86:
    return "call";
  case X86InstrType::NOP_X86:
    return "nop";
  default:
    return "";
  }
}
