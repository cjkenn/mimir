#include <vector>
#include "ir_block.h"

IrBlock::IrBlock() {
  isLeader_ = false;
}

IrBlock::IrBlock(std::vector<Instruction> instructions) {
  isLeader_ = false;
  instructions_ = instructions;
}

void IrBlock::AddInstruction(Instruction instr) {
  instructions_.push_back(instr);
}

void IrBlock::AddAdjacentBlock(IrBlockPtr next) {
  adjacent_.push_back(next);
}
