#pragma once

#include <memory>
#include "ir_instr.h"

class IrBlock;

typedef std::shared_ptr<IrBlock> IrBlockPtr;

class IrBlock {
 public:
  IrBlock();
  IrBlock(std::vector<IrInstr> instructions);
  ~IrBlock() {};

  bool GetIsLeader() const { return isLeader_; }
  std::string GetLabel() const { return label_; }
  std::vector<IrBlockPtr> GetAdjacent() const { return adjacent_; }
  std::vector<IrInstr> GetInstructions() const { return instructions_; }

  void SetLeader(bool isLeader) { isLeader_ = isLeader; }
  void SetLabel(std::string label) { label_ = label; }
  void SetAdjacent(std::vector<IrBlockPtr> adj) { adjacent_ = adj; }
  void SetInstructions(std::vector<IrInstr> instructions) { instructions_ = instructions; }
  void AddInstruction(IrInstr instr);
  void PrependInstruction(IrInstr instr);
  void MergeInstructions(std::vector<IrInstr>& instrs);
  void AddAdjacentBlock(IrBlockPtr next);

 private:
  bool isLeader_;
  std::string label_;
  std::vector<IrBlockPtr> adjacent_;
  std::vector<IrInstr> instructions_;
};
