#pragma once

#include <memory>
#include <utility>
#include <string>
#include "x86_instr_arg.h"

class X86Instr;

typedef std::shared_ptr<X86Instr> X86InstrPtr;
typedef std::shared_ptr<X86InstrArg> X86InstrArgPtr;

enum X86InstrType {
  MOV_X86,
  ADD_X86,
  SUB_X86,
  MUL_X86,
  DIV_X86,
  CMP_X86,
  JMP_X86,
  JE_X86,
  JNE_X86,
  JL_X86,
  JLE_X86,
  JG_X86,
  JGE_X86,
  PUSH_X86,
  POP_X86,
  CALL_X86,
  NOP_X86
};

class X86Instr {
 public:
  X86Instr(X86InstrType type);
  X86Instr(X86InstrType type, std::string label);
  ~X86Instr() {};

  void SetType(X86InstrType type) { type_ = type; }
  void SetArgs(std::pair<X86InstrArgPtr, X86InstrArgPtr> args) { args_ = args; }
  void SetFirstArg(X86InstrArgPtr a) { args_.first = a; }
  void SetSecondArg(X86InstrArgPtr a) { args_.second = a; }

  std::pair<X86InstrArgPtr, X86InstrArgPtr> GetArgs() const { return args_; }
  X86InstrArgPtr GetFirstArg() const { return args_.first; }
  X86InstrArgPtr GetSecondArg() const { return args_.second; }
  X86InstrType GetType() const { return type_; }
  std::string GetLabel() const { return label_; }

  // Convert an instruction object into a string that can be written
  // to an assembly file. Serializing converts the instr type to a string,
  // then appends the first arg and the second arg
  std::string Serialize();

 private:
  std::string MapTypeToStr();

  X86InstrType type_;
  std::pair<X86InstrArgPtr, X86InstrArgPtr> args_;
  std::string label_;
};
