#include <vector>
#include <queue>
#include <assert.h>
#include "x86_instr_sel.h"
#include "cfg_node.h"
#include "x86_instr.h"

void X86InstrSel::ConvertIrInstrs(const CfgNodePtr& block) {
  std::vector<X86InstrPtr> instrs;
  auto ir = block->GetInstrs();

  for (int i = 0; i < ir.size(); i++) {
    auto curr = ir[i];
    instrs.push_back(MapIrToX86(ir, i));
  }

  block->SetX86Instrs(instrs);
}

void X86InstrSel::ConvertIrInstrsForEntireBranch(const CfgNodePtr& block) {
  std::queue<CfgNodePtr> q;
  q.push(block);

  while(!q.empty()) {
    auto block = q.front();
    q.pop();
    block->SetVisited(true);
    ConvertIrInstrs(block);

    for (auto b : block->GetAdj()) {
      if (!b->GetVisited()) {
	q.push(b);
      }
    }
  }
}

X86InstrPtr X86InstrSel::MapIrToX86(const std::vector<IrInstrPtr>& ir, const int i) {
  auto const curr = ir[i];

  switch(curr->GetType()) {
  case IrInstrType::MV_INSTR:
    return ConvertMvInstr(curr);
  case IrInstrType::ADD_INSTR:
  case IrInstrType::SUB_INSTR:
    return ConvertAddSubInstr(curr);
  }
}

X86InstrPtr X86InstrSel::ConvertMvInstr(const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::MV_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  i->SetFirstArg(instr->GetArgs().first);
  i->SetSecondArg(instr->GetArgs().second);

  return i;
}

X86InstrPtr X86InstrSel::ConvertAddSubInstr(const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::ADD_INSTR ||
	 instr->GetType() == IrInstrType::SUB_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::ADD_X86);
  i->SetFirstArg(instr->GetArgs().first);
  i->SetSecondArg(instr->GetArgs().second);

  if (instr->GetType() == IrInstrType::SUB_INSTR) {
    i->SetType(SUB_X86);
  }

  return i;
}
