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
  void MapIrToX86(std::vector<X86InstrPtr>& x86,
		  const std::vector<IrInstrPtr>& ir,
		  const int i);

  // We can basically convert these operations in a one-to-one fashion.
  X86InstrPtr ConvertMvInstr(const IrInstrPtr& instr);
  X86InstrPtr ConvertAddSubInstr(const IrInstrPtr& instr);
  X86InstrPtr ConvertMulInstr(const std::vector<IrInstrPtr>& ir, const int i);

  // Converting DIV instructions is a bit different since we need to insert
  // another MOV instruction to clear out rdx to store the remainder. To accomodate
  // for this we pass in a reference of the current x86 instruction vector and
  // insert into it.
  void ConvertDivInstr(std::vector<X86InstrPtr>& x86,
		       const std::vector<IrInstrPtr>& ir,
		       const int i);
};
