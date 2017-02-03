#pragma once

#include <memory>
#include "instruction.h"

class IrBlock;

typedef std::shared_ptr<IrBlock> IrBlockPtr;

class IrBlock {
 public:
  IrBlock();
  IrBlock(std::vector<Instruction> instructions);
  ~IrBlock() {};

  bool GetIsLeader() const { return isLeader_; }
  std::vector<IrBlockPtr> GetAdjacent() const { return adjacent_; }
  std::vector<Instruction> GetInstructions() const { return instructions_; }

  void SetLeader(bool isLeader) { isLeader_ = isLeader; }
  void SetAdjacent(std::vector<IrBlockPtr> adj) { adjacent_ = adj; }
  void SetInstructions(std::vector<Instruction> instructions) { instructions_ = instructions; }
  void AddInstruction(Instruction instr);
  void PrependInstruction(Instruction instr);
  void AddAdjacentBlock(IrBlockPtr next);

 private:
  bool isLeader_;
  std::vector<IrBlockPtr> adjacent_;
  std::vector<Instruction> instructions_;
};
