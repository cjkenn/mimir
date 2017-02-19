#include <iostream>
#include <vector>
#include "ir_block.h"

IrBlock::IrBlock() {
  isLeader_ = false;
}

IrBlock::IrBlock(std::vector<IrInstr> instructions) {
  isLeader_ = false;
  instructions_ = instructions;
}

void IrBlock::AddInstruction(IrInstr instr) {
  instructions_.push_back(instr);
}

void IrBlock::PrependInstruction(IrInstr instr) {
  instructions_.insert(instructions_.begin(), instr);
}

void IrBlock::AddAdjacentBlock(IrBlockPtr next) {
  adjacent_.push_back(next);
}

void IrBlock::MergeInstructions(std::vector<IrInstr>& instrs) {
  if (instrs.empty()) {
    return;
  }

  instructions_.insert(instructions_.end(), instrs.begin(), instrs.end());
}
