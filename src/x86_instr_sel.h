#pragma once

#include <vector>
#include "ir_instr.h"
#include "cfg_node.h"

class X86InstrSel {
 public:
  X86InstrSel() {};
  ~X86InstrSel() {};

  void ConvertIrInstrs(const CfgNodePtr& block);
  void ConvertIrInstrsForEntireBranch(const CfgNodePtr& block);
 private:
  X86InstrPtr MapIrToX86(const std::vector<IrInstrPtr>& ir, const int i);

  X86InstrPtr ConvertMvInstr(const IrInstrPtr& instr);
  X86InstrPtr ConvertAddSubInstr(const IrInstrPtr& instr);

};
