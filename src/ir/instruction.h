#pragma once

#include <vector>
#include <utility>
#include <string>

enum InstructionType {
  MV_INSTR,
  LD_INSTR,
  SV_INSTR,
  JMP_INSTR,
  JMPLT_INSTR,
  JMPGT_INSTR,
  JMPEQ_INSTR,
  ADD_INSTR,
  SUB_INSTR,
  MUL_INSTR,
  DIV_INSTR,
  CMP_INSTR,
  LBL_INSTR
};

class Instruction {
 public:
  Instruction() {};
  Instruction(InstructionType type, std::string dest);
  Instruction(InstructionType type,
	      std::pair<std::string, std::string> args,
	      std::string dest);
  Instruction(InstructionType type,
	      std::string arg1,
	      std::string arg2,
	      std::string dest);

  ~Instruction() {};

  InstructionType GetType() const { return type_; }
  std::string GetDest() const { return dest_; }
  std::pair<std::string, std::string> GetArgs() const { return args_; }

  void SetType(InstructionType type) { type_ = type; }
  void SetArgs(std::pair<std::string, std::string> args) { args_ = args; }
  void SetDest(std::string dest) { dest_ = dest; }

 private:
  InstructionType type_;
  std::pair<std::string, std::string> args_;
  std::string dest_;
};
