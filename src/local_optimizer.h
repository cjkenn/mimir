#pragma once
#include "cfg_node.h"
#include "ir_instr.h"

class LocalOptimizer {
 public:
  LocalOptimizer() {};
  ~LocalOptimizer() {};

  // Perform local value numbering optimizations,
  // including removing redundant instructions
  // and constant folding.
  void Lvn(CfgNodePtr root);

 private:
  bool IsInstrLvnValid(IrInstrPtr instr);
};
