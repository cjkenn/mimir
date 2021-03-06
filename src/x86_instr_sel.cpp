#include <vector>
#include <queue>
#include <assert.h>
#include <iostream>
#include <memory>
#include "x86_instr_sel.h"
#include "cfg_node.h"
#include "x86_instr.h"
#include "x86_instr_arg.h"
#include "symbol_table.h"
#include "symbol.h"

using namespace mimir;

X86InstrSel::X86InstrSel(std::shared_ptr<SymbolTable> sym_tab) {
  sym_tab_ = sym_tab;
}

void X86InstrSel::SelectInstrs(const CfgNodePtr& block) {
  std::vector<X86InstrPtr> x86;
  auto ir = block->GetInstrs();

  for (unsigned int i = 0; i < ir.size(); i++) {
    auto curr = ir[i];
    MapIrToX86(x86, ir, i);
  }

  block->SetX86Instrs(x86);
}

void X86InstrSel::SelectInstrsForEntireBranch(const CfgNodePtr& block) {
  std::queue<CfgNodePtr> q;
  q.push(block);

  auto ir = block->GetInstrs();

  while(!q.empty()) {
    auto block = q.front();
    q.pop();
    block->SetSelectorVisited(true);
    SelectInstrs(block);

    for (auto b : block->GetAdj()) {
      if (!b->GetSelectorVisited()) {
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
  case IrInstrType::FUNC_ENTER_INSTR:
    ConvertFuncEnterInstr(x86, curr);
    break;
  case IrInstrType::FUNC_EXIT_INSTR:
    ConvertFuncExitInstr(x86, curr);
    break;
  case IrInstrType::FUNC_PARAM_INSTR:
    ConvertParamInstr(x86, curr);
    break;
  case IrInstrType::FUNC_CALL_INSTR:
    ConvertFuncCallInstr(x86, curr);
    break;
  default:
    assert(false);
    break;
  }
}

void X86InstrSel::ConvertMvInstr(std::vector<X86InstrPtr>& x86,
				 const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::MV_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::MOV_X86, instr->GetLabel());

  // The order of args is swapped around in x86, compared to the ir
  auto arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
					    instr->GetArgs().second);
  i->SetFirstArg(arg1);

  auto arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::NUM_X86,
					    instr->GetArgs().first);
  i->SetSecondArg(arg2);

  x86.push_back(i);
}

void X86InstrSel::ConvertLdInstr(std::vector<X86InstrPtr>& x86,
				 const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::LD_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::MOV_X86, instr->GetLabel());

  auto arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86, instr->GetDest());
  i->SetFirstArg(arg1);

  const std::string name = instr->GetArgs().first;
  // TODO: Shouldn't really be calling the symbol table here (or in ConvertSv), because our
  // scopes are opened and closed in the parser. We're not sure what scope we're in here, which
  // may cause problems and we shouldn't change the parser to accomodate that.
  // Maybe the scope and symbol info should be encoded in the argument in the IR (we can expand
  // the IR as much as we want)
  const SymbolPtr sym = sym_tab_->Find(name);
  assert(sym != nullptr);

  const int offset = sym->GetStackOffset();
  auto arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::ADDR_X86,
					    BuildLdAddressArg(offset));
  i->SetSecondArg(arg2);

  x86.push_back(i);
}

void X86InstrSel::ConvertSvInstr(std::vector<X86InstrPtr>& x86,
				 const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::SV_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::MOV_X86, instr->GetLabel());
  const std::string name = instr->GetArgs().second;

  const SymbolPtr sym = sym_tab_->Find(name);
  assert(sym != nullptr);

  const int offset = sym->GetStackOffset();
  auto arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::ADDR_X86,
					    BuildLdAddressArg(offset));
  i->SetFirstArg(arg1);

  auto arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86, instr->GetArgs().first);
  i->SetSecondArg(arg2);

  x86.push_back(i);
}

void X86InstrSel::ConvertAddSubInstr(std::vector<X86InstrPtr>& x86,
				     const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::ADD_INSTR ||
	 instr->GetType() == IrInstrType::SUB_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::ADD_X86, instr->GetLabel());
  auto arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
					    instr->GetArgs().second);
  i->SetFirstArg(arg1);

  auto arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
					    instr->GetArgs().first);
  i->SetSecondArg(arg2);

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
  X86InstrPtr mul_reg = std::make_shared<X86Instr>(X86InstrType::MOV_X86, curr->GetLabel());

  auto mul_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86, "rax");
  mul_reg->SetFirstArg(mul_arg1);

  auto mul_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
					    curr->GetArgs().first);
  mul_reg->SetSecondArg(mul_arg2);
  x86.push_back(mul_reg);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::MUL_X86, curr->GetLabel());

  auto arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
					    curr->GetArgs().second);
  instr->SetFirstArg(arg1);

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
  X86InstrPtr clr = std::make_shared<X86Instr>(X86InstrType::MOV_X86, curr->GetLabel());
  auto clr_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86, "rdx");
  clr->SetFirstArg(clr_arg1);

  auto clr_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::NUM_X86, "0");
  clr->SetSecondArg(clr_arg2);
  x86.push_back(clr);

  X86InstrPtr div_reg = std::make_shared<X86Instr>(X86InstrType::MOV_X86, curr->GetLabel());

  auto div_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86, "rax");
  div_reg->SetFirstArg(div_arg1);

  auto div_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						curr->GetArgs().first);
  div_reg->SetSecondArg(div_arg2);
  x86.push_back(div_reg);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::DIV_X86, curr->GetLabel());

  auto arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						curr->GetArgs().second);
  instr->SetFirstArg(arg1);
  x86.push_back(instr);
}

void X86InstrSel::ConvertModInstr(std::vector<X86InstrPtr>& x86,
				  const std::vector<IrInstrPtr>& ir,
				  const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::MOD_INSTR);

  // TODO: Again, should these clr registers be provided by the allocator?
  X86InstrPtr clr = std::make_shared<X86Instr>(X86InstrType::MOV_X86, curr->GetLabel());
  auto const clr_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86, "rdx");
  clr->SetFirstArg(clr_arg1);

  auto const clr_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::NUM_X86, "0");
  clr->SetSecondArg(clr_arg2);

  x86.push_back(clr);

  X86InstrPtr div_reg = std::make_shared<X86Instr>(X86InstrType::MOV_X86, curr->GetLabel());
  auto const div_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86, "rax");
  div_reg->SetFirstArg(div_arg1);

  auto const div_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						      curr->GetArgs().first);
  div_reg->SetSecondArg(div_arg2);
  x86.push_back(div_reg);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::DIV_X86, curr->GetLabel());
  auto const instr_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						      curr->GetArgs().second);
  instr->SetFirstArg(instr_arg1);
  x86.push_back(instr);

  X86InstrPtr get_remainder = std::make_shared<X86Instr>(X86InstrType::MOV_X86, curr->GetLabel());
  auto const rem_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						      "rax");
  get_remainder->SetFirstArg(rem_arg1);

  auto const rem_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						      "rdx");
  get_remainder->SetSecondArg(rem_arg2);
  x86.push_back(get_remainder);
}


void X86InstrSel::ConvertCmpInstr(std::vector<X86InstrPtr>& x86,
				  const std::vector<IrInstrPtr>& ir,
				  const int i) {
  auto const curr = ir[i];
  assert(curr->GetType() == IrInstrType::CMP_INSTR);

  X86InstrPtr instr = std::make_shared<X86Instr>(X86InstrType::CMP_X86, curr->GetLabel());
  auto const instr_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						      curr->GetArgs().first);
  instr->SetFirstArg(instr_arg1);

  auto const instr_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::REG_X86,
						      curr->GetArgs().second);
  instr->SetSecondArg(instr_arg2);
  x86.push_back(instr);
}

void X86InstrSel::ConvertBranchingInstr(std::vector<X86InstrPtr>& x86,
					const IrInstrPtr& instr) {
  X86InstrType j_type = GetX86BranchType(instr);
  X86InstrPtr i = std::make_shared<X86Instr>(j_type, instr->GetLabel());
  auto const instr_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::LBL_X86,
							instr->GetDest());
  i->SetFirstArg(instr_arg1);
  x86.push_back(i);
}

void X86InstrSel::ConvertNopInstr(std::vector<X86InstrPtr>& x86,
				  const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::NOP_INSTR);

  X86InstrPtr i = std::make_shared<X86Instr>(X86InstrType::NOP_X86, instr->GetLabel());
  x86.push_back(i);
}

void X86InstrSel::ConvertFuncEnterInstr(std::vector<X86InstrPtr>& x86,
					const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::FUNC_ENTER_INSTR);

  auto push_instr = std::make_shared<X86Instr>(X86InstrType::PUSH_X86, instr->GetLabel());
  auto const push_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::RESERVED_REG_X86,
						      "rbp");
  push_instr->SetFirstArg(push_arg1);
  x86.push_back(push_instr);

  auto mv_instr = std::make_shared<X86Instr>(X86InstrType::MOV_X86, instr->GetLabel());
  auto const mv_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::RESERVED_REG_X86,
						     "rbp");

  auto const mv_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::RESERVED_REG_X86,
						     "rsp");
  mv_instr->SetFirstArg(mv_arg1);
  mv_instr->SetSecondArg(mv_arg2);
  x86.push_back(mv_instr);

  // TODO: Get stack size needed by this function. Should be available through the sym table,
  // under the scope for this function (if we can find it)
  const unsigned int stack_sz = 0;

  if (stack_sz > 0) {
    auto sub_instr = std::make_shared<X86Instr>(X86InstrType::SUB_X86, instr->GetLabel());
    auto const sub_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::RESERVED_REG_X86,
							 "rsp");
    sub_instr->SetFirstArg(sub_arg1);

    auto const sub_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::NUM_X86,
							 std::to_string(stack_sz));
    sub_instr->SetSecondArg(sub_arg2);
    x86.push_back(sub_instr);
  }
}

void X86InstrSel::ConvertFuncExitInstr(std::vector<X86InstrPtr>& x86,
				       const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::FUNC_EXIT_INSTR);

  auto mv_instr = std::make_shared<X86Instr>(X86InstrType::MOV_X86, instr->GetLabel());
  auto const mv_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::RESERVED_REG_X86,
						      "rsp");
  mv_instr->SetFirstArg(mv_arg1);

  auto const mv_arg2 = std::make_shared<X86InstrArg>(X86InstrArgType::RESERVED_REG_X86,
						      "rbp");
  mv_instr->SetSecondArg(mv_arg2);
  x86.push_back(mv_instr);

  auto pop_instr = std::make_shared<X86Instr>(X86InstrType::POP_X86, instr->GetLabel());
  auto const pop_arg1 = std::make_shared<X86InstrArg>(X86InstrArgType::RESERVED_REG_X86,
						      "rbp");
  pop_instr->SetFirstArg(pop_arg1);
  x86.push_back(pop_instr);

  // TODO: Support for returning values instead of an empty return
  auto ret_instr = std::make_shared<X86Instr>(X86InstrType::RET_X86, instr->GetLabel());

  x86.push_back(ret_instr);
}

void X86InstrSel::ConvertFuncCallInstr(std::vector<X86InstrPtr>& x86,
				       const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::FUNC_CALL_INSTR);

  auto call_instr = std::make_shared<X86Instr>(X86InstrType::CALL_X86, instr->GetLabel());
  auto const func_name = std::make_shared<X86InstrArg>(X86InstrArgType::LBL_X86,
						       instr->GetArgs().first);
  call_instr->SetFirstArg(func_name);
  x86.push_back(call_instr);
}

void X86InstrSel::ConvertParamInstr(std::vector<X86InstrPtr>& x86,
				    const IrInstrPtr& instr) {
  assert(instr->GetType() == IrInstrType::FUNC_PARAM_INSTR);

  auto push_instr = std::make_shared<X86Instr>(X86InstrType::PUSH_X86, instr->GetLabel());
  // TODO: Needs to handle different param types other than numbers. We need to load
  // the variables that we pass to the function.
  auto const param_arg = std::make_shared<X86InstrArg>(X86InstrArgType::NUM_X86,
					      instr->GetArgs().first);
  push_instr->SetFirstArg(param_arg);
  x86.push_back(push_instr);
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

std::string X86InstrSel::BuildDataAddressArg(const std::string name) {
  return "[" + name + "]";
}

std::string X86InstrSel::BuildLdAddressArg(const int offset) {
  return "[rbp-" + std::to_string(offset) + "]";
}
