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

  void ConvertMvInstr(std::vector<X86InstrPtr>& x86,
		      const IrInstrPtr& instr);

  void ConvertAddSubInstr(std::vector<X86InstrPtr>& x86,
			  const IrInstrPtr& instr);

  void ConvertMulInstr(std::vector<X86InstrPtr>& x86,
		       const std::vector<IrInstrPtr>& ir,
		       const int i);

  void ConvertDivInstr(std::vector<X86InstrPtr>& x86,
		       const std::vector<IrInstrPtr>& ir,
		       const int i);

  void ConvertCmpInstr(std::vector<X86InstrPtr>& x86,
		       const std::vector<IrInstrPtr>& ir,
		       const int i);

  void ConvertBranchingInstr(std::vector<X86InstrPtr>& x86,
			     const IrInstrPtr& instr);

  void ConvertNopInstr(std::vector<X86InstrPtr>& x86,
		       const IrInstrPtr& instr);

  X86InstrType GetX86BranchType(IrInstrPtr instr);
};
