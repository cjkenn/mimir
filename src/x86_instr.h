#pragma once

#include <memory>
#include <utility>

class X86Instr;

typedef std::shared_ptr<X86Instr> X86InstrPtr;

enum X86InstrType {
  MOV_INSTR,
  ADD_INSTR,
  SUB_INSTR,
  MUL_INSTR,
  DIV_INSTR,
  CMP_INSTR,
  JMP_INSTR,
  JE_INSTR,
  JNE_INSTR,
  JL_INSTR,
  JLE_INSTR,
  JG_INSTR,
  JGE_INSTR,
  PUSH_INSTR,
  POP_INSTR,
  CALL_INSTR
};

class X86Instr {
 public:
  X86Instr(X86InstrType type);
  ~X86Instr() {};

 private:
  X86InstrType type_;
  std::pair<std::string, std::string> args_;
};
