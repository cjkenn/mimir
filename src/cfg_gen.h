#pragma once

#include "cfg.h"
#include "ir_instr.h"

class CfgGen {
 public:
  CfgGen();
  ~CfgGen() {};

  // Create a cfg from an ir.
  Cfg Gen(const std::vector<IrInstrPtr>& ir);

 private:
  std::string GetNextName();

  // Maps the label of the instruction to an index of a block
  // in an array. This intended to be called with jmp isntructions.
  // For example, if the destination of the instruction is lbl1, this
  // returns 1.
  int FindBlockIndex(IrInstrPtr instr);

  // The name of the current block.
  std::string curr_name_;

  // How many blocks we've created (0 indexed)
  int block_count_;
};
