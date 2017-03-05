#include <vector>
#include <queue>
#include <assert.h>
#include "x86_instr_sel.h"
#include "cfg_node.h"
#include "x86_instr.h"

void X86InstrSel::ConvertIrInstrs(const CfgNodePtr& block) {
  std::vector<X86InstrPtr> x86;
  auto ir = block->GetInstrs();

  for (int i = 0; i < ir.size(); i++) {
    auto curr = ir[i];
    MapIrToX86(x86, ir, i);
  }

  block->SetX86Instrs(x86);
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

void X86InstrSel::MapIrToX86(std::vector<X86InstrPtr>& x86,
			     const std::vector<IrInstrPtr>& ir,
			     const int i) {
  auto const curr = ir[i];

  switch(curr->GetType()) {
  case IrInstrType::MV_INSTR:
    x86.push_back(ConvertMvInstr(curr));
  case IrInstrType::ADD_INSTR:
  case IrInstrType::SUB_INSTR:
    x86.push_back(ConvertAddSubInstr(curr));
  case IrInstrType::MUL_INSTR:
    x86.push_back(ConvertMulInstr(ir, i));
  case IrInstrType::DIV_INSTR:
    ConvertDivInstr(x86, ir, i);
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
    i->SetType(X86InstrType::SUB_X86);
  }

  return i;
}

X86InstrPtr X86InstrSel::ConvertMulInstr(const std::vector<IrInstrPtr>& ir, const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::MUL_INSTR);
  assert(i > 1);

  // MUL instructions in x86 should have the first operand in rax. Then the result
  // will be returned there, with one argument provided to the instruction
  // TODO: This should be built into the register allocator. ** I think **
  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::MUL_X86);
  instr->SetFirstArg(ir[i-1]->GetArgs().second);

  return instr;
}

void X86InstrSel::ConvertDivInstr(std::vector<X86InstrPtr>& x86,
				  const std::vector<IrInstrPtr>& ir,
				  const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::DIV_INSTR);
  assert(i > 1);

  // DIV instructions will have the dividend in rax and the divisor in any other reg.
  // We must clear rdx first to store the remainder there.
  // TODO: Again handled by the register allocator?? Might need to do some work here
  X86InstrPtr clr = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  clr->SetFirstArg("rdx");
  clr->SetSecondArg("0");
  x86.push_back(clr);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::DIV_X86);
  instr->SetFirstArg(ir[i-1]->GetArgs().second);
  x86.push_back(instr);
}
