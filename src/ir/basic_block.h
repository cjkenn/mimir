#pragma once

#include <memory>

enum BasicBlockInstr {
  MV_INSTR,
  JMP_INSTR,
  JMPZ_INSTR,
  ADD_INSTR,
  SUB_INSTR,
  MUL_INSTR,
  DIV_INSTR
};

class BasicBlock {
 public:
  BasicBlock(BasicBlockInstr instr,
	     std::string arg1,
	     std::string arg2,
	     std::string dest);

  BasicBlock(BasicBlockInstr instr,
	     std::string arg1,
	     std::string arg2,
	     std::string dest,
	     std::shared_ptr<BasicBlock> next);

  ~BasicBlock() {};

  std::string GetFirstArg() const { return first_arg_; }
  std::string GetSecondArg() const { return second_arg_; }
  std::string GetDest() const { return dest_; }
  bool GetIsLeader() const { return isLeader_; }
  std::shared_ptr<BasicBlock> GetNext() const { return next_block_; }

  void SetFirstArg(std::string arg) { first_arg_ = arg; }
  void SetSecondArg(std::string arg) { second_arg_ = arg; }
  void SetDest(std::string dest) { dest_ = dest; }
  void SetLeader(bool isLeader) { isLeader_ = isLeader; }
  void SetNext(std::shared_ptr<BasicBlock> next) { next_block_ = next; }

 private:
  BasicBlockInstr instr_;
  std::string first_arg_;
  std::string second_arg_;
  std::string dest_;
  std::string label_;
  bool isLeader_;
  std::shared_ptr<BasicBlock> next_block_;
};
