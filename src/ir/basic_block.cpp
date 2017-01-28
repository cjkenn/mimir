#include "basic_block.h"

BasicBlock::BasicBlock(BasicBlockInstr instr,
		       std::string arg1,
		       std::string arg2,
		       std::string dest) {
  instr_ = instr;
  first_arg_ = arg1;
  second_arg_ = arg2;
  dest_ = dest;
  isLeader_ = false;
  next_block_ = nullptr;
}

BasicBlock::BasicBlock(BasicBlockInstr instr,
		       std::string arg1,
		       std::string arg2,
		       std::string dest,
		       BasicBlockPtr next) {
  instr_ = instr;
  first_arg_ = arg1;
  second_arg_ = arg2;
  dest_ = dest;
  isLeader_ = false;
  next_block_ = next;
}
