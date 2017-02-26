#pragma once

#include <string>
#include <unordered_map>
#include "cfg_node.h"
#include "ir_instr.h"

class LocalOptimizer {
 public:
  LocalOptimizer();
  ~LocalOptimizer() {};

  // Perform local value numbering optimizations,
  // including removing redundant instructions
  // and constant folding. This method operates only on
  // the block provided. If local optimizations are needed
  // on all blocks, this must be called for each of them
  // (but the order of those calls is not important, since only
  // the block itself is affected).
  void Lvn(CfgNodePtr root);

 private:
  // We only care about certain instructions for local optimizations. There
  // is no reason to analyze compares or jumps or nops, so we filter them out
  // with this method.
  bool IsInstrLvnValid(IrInstrPtr instr);

  // Determine the lvn value for the first argument to an instruction.
  int GetLvnForFirstArg(IrInstrPtr instr);

  // Similar to the above method, but with some extra checking to ensure
  // that LD instruction arguments have the correct values.
  int GetLvnForSecondArg(IrInstrPtr instr, int val1);

  // We must construct a string to act as the key for the op_map.
  // The key takes the form of "V1OPV2". For example,
  // the operation "add r0, r1", with v(r0) = 0 and v(r1) = 1, will result in
  // a key of "0ADD1".
  std::string BuildLvnMapKey(IrInstrPtr instr, int val1, int val2);

  int val_count_;
  std::unordered_map<std::string, int> val_map_;
  std::unordered_map<std::string, std::string> op_map_;
};
