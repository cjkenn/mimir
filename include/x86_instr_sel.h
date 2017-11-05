#pragma once

#include <vector>
#include "ir_instr.h"
#include "cfg_node.h"
#include "symbol_table.h"
#include "x86_reg_alloc.h"

class X86InstrSel {
 public:
  X86InstrSel(std::shared_ptr<SymbolTable> sym_tab);
  ~X86InstrSel() {};

  void SelectInstrs(const CfgNodePtr& block);
  void SelectInstrsForEntireBranch(const CfgNodePtr& block);
 private:
  void MapIrToX86(std::vector<X86InstrPtr>& x86,
		  const std::vector<IrInstrPtr>& ir,
		  const int i);

  void ConvertMvInstr(std::vector<X86InstrPtr>& x86,
		      const IrInstrPtr& instr);

  void ConvertLdInstr(std::vector<X86InstrPtr>& x86,
		      const IrInstrPtr& instr);

  void ConvertSvInstr(std::vector<X86InstrPtr>& x86,
		      const IrInstrPtr& instr);

  void ConvertAddSubInstr(std::vector<X86InstrPtr>& x86,
			  const IrInstrPtr& instr);

  void ConvertMulInstr(std::vector<X86InstrPtr>& x86,
		       const std::vector<IrInstrPtr>& ir,
		       const int i);

  void ConvertDivInstr(std::vector<X86InstrPtr>& x86,
		       const std::vector<IrInstrPtr>& ir,
		       const int i);

  void ConvertModInstr(std::vector<X86InstrPtr>& x86,
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

 private:
  std::string BuildDataAddressArg(const std::string name);
  std::string BuildLdAddressArg(const int offset);
  std::shared_ptr<SymbolTable> sym_tab_;
};
