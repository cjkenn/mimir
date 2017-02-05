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

void IrBlock::PrependInstruction(Instruction instr) {
  instructions_.insert(instructions_.begin(), instr);
}

void IrBlock::AddAdjacentBlock(IrBlockPtr next) {
  adjacent_.push_back(next);
}

void IrBlock::MergeInstructions(std::vector<Instruction> instrs) {
  instructions_.insert(instructions_.end(), instrs.begin(), instrs.end());
}
