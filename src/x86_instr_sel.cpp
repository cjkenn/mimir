#include <vector>
#include <queue>
#include <assert.h>
#include <iostream>
#include "x86_instr_sel.h"
#include "cfg_node.h"
#include "x86_instr.h"
#include "symbol_table.h"
#include "symbol.h"

X86InstrSel::X86InstrSel(std::shared_ptr<SymbolTable> sym_tab) {
  sym_tab_ = sym_tab;
}

void X86InstrSel::SelectInstrs(const CfgNodePtr& block) {
  std::vector<X86InstrPtr> x86;
  auto ir = block->GetInstrs();

  for (int i = 0; i < ir.size(); i++) {
    auto curr = ir[i];
    MapIrToX86(x86, ir, i);
  }

  block->SetX86Instrs(x86);
}

void X86InstrSel::SelectInstrsForEntireBranch(const CfgNodePtr& block) {
  std::queue<CfgNodePtr> q;
  q.push(block);

  while(!q.empty()) {
    auto block = q.front();
    q.pop();
    block->SetVisited(true);
    SelectInstrs(block);

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
    ConvertMvInstr(x86, curr);
    break;
  case IrInstrType::LD_INSTR:
    ConvertLdInstr(x86, curr);
    break;
  case IrInstrType::SV_INSTR:
    ConvertSvInstr(x86, curr);
    break;
  case IrInstrType::ADD_INSTR:
  case IrInstrType::SUB_INSTR:
    ConvertAddSubInstr(x86, curr);
    break;
  case IrInstrType::MUL_INSTR:
    ConvertMulInstr(x86, ir, i);
    break;
  case IrInstrType::DIV_INSTR:
    ConvertDivInstr(x86, ir, i);
    break;
  case IrInstrType::MOD_INSTR:
    ConvertModInstr(x86, ir, i);
    break;
  case IrInstrType::CMP_INSTR:
    ConvertCmpInstr(x86, ir, i);
    break;
  case IrInstrType::JMP_INSTR:
  case IrInstrType::JMPLT_INSTR:
  case IrInstrType::JMPLTE_INSTR:
  case IrInstrType::JMPGT_INSTR:
  case IrInstrType::JMPGTE_INSTR:
  case IrInstrType::JMPEQ_INSTR:
  case IrInstrType::JMPNEQ_INSTR:
    ConvertBranchingInstr(x86, curr);
    break;
  case IrInstrType::NOP_INSTR:
    ConvertNopInstr(x86, curr);
    break;
  }
}

void X86InstrSel::ConvertMvInstr(std::vector<X86InstrPtr>& x86,
				 const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::MV_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  // The order of args is swapped around in x86, compared to the ir
  i->SetFirstArg(instr->GetArgs().second);
  i->SetSecondArg(instr->GetArgs().first);

  x86.push_back(i);
}

void X86InstrSel::ConvertLdInstr(std::vector<X86InstrPtr>& x86,
				 const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::LD_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  i->SetFirstArg(instr->GetDest());
  const std::string name = instr->GetArgs().first;

  const SymbolPtr sym = sym_tab_->Find(name);
  assert(sym != nullptr);

  const int offset = sym->GetStackOffset();
  i->SetSecondArg(BuildLdAddressArg(offset));
  x86.push_back(i);
}

void X86InstrSel::ConvertSvInstr(std::vector<X86InstrPtr>& x86,
				 const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::SV_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  const std::string name = instr->GetArgs().second;

  const SymbolPtr sym = sym_tab_->Find(name);
  assert(sym != nullptr);

  const int offset = sym->GetStackOffset();
  i->SetFirstArg(BuildLdAddressArg(offset));
  i->SetSecondArg(instr->GetArgs().first);
  x86.push_back(i);
}

void X86InstrSel::ConvertAddSubInstr(std::vector<X86InstrPtr>& x86,
				     const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::ADD_INSTR ||
	 instr->GetType() == IrInstrType::SUB_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::ADD_X86);
  i->SetFirstArg(instr->GetArgs().second);
  i->SetSecondArg(instr->GetArgs().first);

  if (instr->GetType() == IrInstrType::SUB_INSTR) {
    i->SetType(X86InstrType::SUB_X86);
  }

  x86.push_back(i);
}

void X86InstrSel::ConvertMulInstr(std::vector<X86InstrPtr>& x86,
				  const std::vector<IrInstrPtr>& ir,
				  const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::MUL_INSTR);

  // MUL instructions in x86 should have the first operand in rax. Then the result
  // will be returned there, with one argument provided to the instruction
  // TODO: This should be built into the register allocator eventually too
  X86InstrPtr mul_reg = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  mul_reg->SetFirstArg("rax");
  mul_reg->SetSecondArg(curr->GetArgs().first);
  x86.push_back(mul_reg);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::MUL_X86);
  instr->SetFirstArg(curr->GetArgs().second);

  x86.push_back(instr);
}

void X86InstrSel::ConvertDivInstr(std::vector<X86InstrPtr>& x86,
				  const std::vector<IrInstrPtr>& ir,
				  const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::DIV_INSTR);

  // DIV instructions will have the dividend in rax and the divisor in any other reg.
  // We must clear rdx first to store the remainder there.
  // TODO: Again handled by the register allocator?? Might need to do some work here
  X86InstrPtr clr = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  clr->SetFirstArg("rdx");
  clr->SetSecondArg("0");
  x86.push_back(clr);

  X86InstrPtr div_reg = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  div_reg->SetFirstArg("rax");
  div_reg->SetSecondArg(curr->GetArgs().first);
  x86.push_back(div_reg);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::DIV_X86);
  instr->SetFirstArg(curr->GetArgs().second);
  x86.push_back(instr);
}

void X86InstrSel::ConvertModInstr(std::vector<X86InstrPtr>& x86,
				  const std::vector<IrInstrPtr>& ir,
				  const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::MOD_INSTR);

  X86InstrPtr clr = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  clr->SetFirstArg("rdx");
  clr->SetSecondArg("0");
  x86.push_back(clr);

  X86InstrPtr div_reg = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  div_reg->SetFirstArg("rax");
  div_reg->SetSecondArg(curr->GetArgs().first);
  x86.push_back(div_reg);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::DIV_X86);
  instr->SetFirstArg(curr->GetArgs().second);
  x86.push_back(instr);

  X86InstrPtr get_remainder = std::make_shared<X86Instr>(X86InstrType::MOV_X86);
  get_remainder->SetFirstArg("rax");
  get_remainder->SetSecondArg("rdx");
  x86.push_back(get_remainder);
}


void X86InstrSel::ConvertCmpInstr(std::vector<X86InstrPtr>& x86,
				  const std::vector<IrInstrPtr>& ir,
				  const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::CMP_INSTR);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::CMP_X86);
  instr->SetFirstArg(curr->GetArgs().first);
  instr->SetSecondArg(curr->GetArgs().second);
  x86.push_back(instr);
}

void X86InstrSel::ConvertBranchingInstr(std::vector<X86InstrPtr>& x86,
					const IrInstrPtr& instr) {
  X86InstrType j_type = GetX86BranchType(instr);

  X86InstrPtr i = std::make_shared<X86Instr>(j_type);
  i->SetFirstArg(instr->GetDest());
  x86.push_back(i);
}

void X86InstrSel::ConvertNopInstr(std::vector<X86InstrPtr>& x86,
				  const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::NOP_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::NOP_X86);
  x86.push_back(i);
}

X86InstrType X86InstrSel::GetX86BranchType(IrInstrPtr instr) {
  assert(instr->IsJmp());

  switch(instr->GetType()) {
  case IrInstrType::JMP_INSTR:
    return X86InstrType::JMP_X86;
  case IrInstrType::JMPEQ_INSTR:
    return X86InstrType::JE_X86;
  case IrInstrType::JMPNEQ_INSTR:
    return X86InstrType::JNE_X86;
  case IrInstrType::JMPGT_INSTR:
    return X86InstrType::JG_X86;
  case IrInstrType::JMPGTE_INSTR:
    return X86InstrType::JGE_X86;
  case IrInstrType::JMPLT_INSTR:
    return X86InstrType::JL_X86;
  case IrInstrType::JMPLTE_INSTR:
    return X86InstrType::JLE_X86;
  default:
    return X86InstrType::JMP_X86;
  }
}

std::string X86InstrSel::BuildLdAddressArg(const int offset) {
  return "[ebp-" + std::to_string(offset) + "]";
}
