#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>
#include "x86_instr_sel.h"
#include "symbol_table.h"
#include "symbol.h"
#include "ir_instr.h"
#include "x86_instr.h"
#include "cfg_node.h"
#include "ast.h"

const unsigned int test_reg_cnt = 10; // ensures there's always enough regs, for now

// Input:
//
// sv 10, x
//
// Expected Output:
//
// mv [ebp-4], 10
void test_select_instrs_simple_assign(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr sv = std::make_shared<IrInstr>(IrInstrType::SV_INSTR,
					    "10",
					    "x",
					    "x");
  instrs.push_back(sv);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 1);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "[rbp-8]");
  assert(first_x86->GetSecondArg()->GetVal() == "10");
}

// Input:
//
// sv 10, x
// sv 9, y
//
// Expected Output:
//
// mv [ebp-4], 10
// mv [ebp-8], 9
void test_select_instrs_two_assigns(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr sv = std::make_shared<IrInstr>(IrInstrType::SV_INSTR,
					    "10",
					    "x",
					    "x");

  IrInstrPtr sv2 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR,
					    "9",
					    "y",
					    "y");
  instrs.push_back(sv);
  instrs.push_back(sv2);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 2);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "[rbp-8]");
  assert(first_x86->GetSecondArg()->GetVal() == "10");

  auto second_x86 = cfg_node->GetX86Instrs()[1];
  assert(second_x86->GetType() == X86InstrType::MOV_X86);
  assert(second_x86->GetFirstArg()->GetVal() == "[rbp-16]");
  assert(second_x86->GetSecondArg()->GetVal() == "9");
}

// Input:
//
// ld x, r0
//
// Expected Output:
//
// mv r0, [ebp-4]
void test_select_instrs_simple_access(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr ld = std::make_shared<IrInstr>(IrInstrType::LD_INSTR,
					    "x",
					    "r0",
					    "r0");
  instrs.push_back(ld);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 1);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "r0");
  assert(first_x86->GetSecondArg()->GetVal() == "[rbp-8]");
}

// Input:
//
// ld x, r0
// ld y, r1
//
// Expected Output:
//
// mv r0, [ebp-4]
// mv r1, [ebp-8]
void test_select_instrs_multiple_access(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr ld = std::make_shared<IrInstr>(IrInstrType::LD_INSTR,
					    "x",
					    "r0",
					    "r0");

  IrInstrPtr ld2 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR,
					    "y",
					    "r1",
					    "r1");
  instrs.push_back(ld);
  instrs.push_back(ld2);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 2);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "r0");
  assert(first_x86->GetSecondArg()->GetVal() == "[rbp-8]");

  auto second_x86 = cfg_node->GetX86Instrs()[1];
  assert(second_x86->GetType() == X86InstrType::MOV_X86);
  assert(second_x86->GetFirstArg()->GetVal() == "r1");
  assert(second_x86->GetSecondArg()->GetVal() == "[rbp-16]");
}

// Input:
//
// mv 10, r0
//
// Expected Output:
//
// mov rax, 10
void test_select_instrs_mv_instr(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr mv = std::make_shared<IrInstr>(IrInstrType::MV_INSTR,
					    "10",
					    "r0",
					    "r0");
  instrs.push_back(mv);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 1);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "rax");
  assert(first_x86->GetSecondArg()->GetVal() == "10");
}

// Input:
//
// add r0, r1 ->expected dest is r1
//
// Expected Output:
//
// add r1, r0 ->have to swap arguments for destination
void test_select_instrs_add_instr(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr add = std::make_shared<IrInstr>(IrInstrType::ADD_INSTR,
					    "r0",
					    "r1",
					    "r1");
  instrs.push_back(add);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 1);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::ADD_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "r1");
  assert(first_x86->GetSecondArg()->GetVal() == "r0");
}

// Input:
//
// mul r0, r1
//
// Expected Output:
//
// mov rax, r0
// mul r1
void test_select_instrs_mul_instr(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr mul = std::make_shared<IrInstr>(IrInstrType::MUL_INSTR,
					     "r0",
					     "r1",
					     "r1");
  instrs.push_back(mul);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 2);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "rax");
  assert(first_x86->GetSecondArg()->GetVal() == "r0");

  auto second_x86 = cfg_node->GetX86Instrs()[1];
  assert(second_x86->GetType() == X86InstrType::MUL_X86);
  assert(second_x86->GetFirstArg()->GetVal() == "r1");
}

// Input:
//
// div r0, r1
//
// Expected Output:
//
// mov rdx, 0
// mov rax, r0
// div r1
void test_select_instrs_div_instr(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr div = std::make_shared<IrInstr>(IrInstrType::DIV_INSTR,
					     "r0",
					     "r1",
					     "r1");
  instrs.push_back(div);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 3);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "rdx");
  assert(first_x86->GetSecondArg()->GetVal() == "0");

  auto second_x86 = cfg_node->GetX86Instrs()[1];
  assert(second_x86->GetType() == X86InstrType::MOV_X86);
  assert(second_x86->GetFirstArg()->GetVal() == "rax");
  assert(second_x86->GetSecondArg()->GetVal() == "r0");

  auto third_x86 = cfg_node->GetX86Instrs()[2];
  assert(third_x86->GetType() == X86InstrType::DIV_X86);
  assert(third_x86->GetFirstArg()->GetVal() == "r1");
}

// Input:
//
// mod r0, r1
//
// Expected Output:
//
// mov rdx, 0
// mov rax, r0
// div r1
// mov rax, rdx
void test_select_instrs_mod_instr(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr mod = std::make_shared<IrInstr>(IrInstrType::MOD_INSTR,
					     "r0",
					     "r1",
					     "r1");
  instrs.push_back(mod);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 4);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "rdx");
  assert(first_x86->GetSecondArg()->GetVal() == "0");

  auto second_x86 = cfg_node->GetX86Instrs()[1];
  assert(second_x86->GetType() == X86InstrType::MOV_X86);
  assert(second_x86->GetFirstArg()->GetVal() == "rax");
  assert(second_x86->GetSecondArg()->GetVal() == "r0");

  auto third_x86 = cfg_node->GetX86Instrs()[2];
  assert(third_x86->GetType() == X86InstrType::DIV_X86);
  assert(third_x86->GetFirstArg()->GetVal() == "r1");

  auto fourth_x86 = cfg_node->GetX86Instrs()[3];
  assert(fourth_x86->GetType() == X86InstrType::MOV_X86);
  assert(fourth_x86->GetFirstArg()->GetVal() == "rax");
  assert(fourth_x86->GetSecondArg()->GetVal() == "rdx");
}

// Input:
//
// cmp x, 10
//
// Expected Output:
//
// cmp x, 10
void test_select_instrs_cmp(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr cmp = std::make_shared<IrInstr>(IrInstrType::CMP_INSTR,
					    "x",
					    "10",
					    "x");
  instrs.push_back(cmp);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 1);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::CMP_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "x");
  assert(first_x86->GetSecondArg()->GetVal() == "10");
}

// Input:
//
// jmp lbl0
//
// Expected Output:
//
// jmp lbl0
void test_select_instrs_simple_branch(std::shared_ptr<SymbolTable> sym_tab) {
  std::vector<IrInstrPtr> instrs;
  IrInstrPtr jmp = std::make_shared<IrInstr>(IrInstrType::JMP_INSTR);
  jmp->SetDest("lbl0");
  instrs.push_back(jmp);

  CfgNodePtr cfg_node = std::make_shared<CfgNode>("b0");
  cfg_node->SetInstrs(instrs);

  X86InstrSel x86is(sym_tab, test_reg_cnt);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 1);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::JMP_X86);
  assert(first_x86->GetFirstArg()->GetVal() == "lbl0");
}


int main(int argc, char **argv) {
  auto sym_tab = std::make_shared<SymbolTable>();
  auto x_node = std::make_shared<AstNode>(AstType::VAR_AST);
  x_node->SetText("x");
  auto y_node = std::make_shared<AstNode>(AstType::VAR_AST);
  y_node->SetText("y");

  sym_tab->Insert(x_node);
  sym_tab->Insert(y_node);

  test_select_instrs_simple_assign(sym_tab);
  test_select_instrs_two_assigns(sym_tab);
  test_select_instrs_simple_access(sym_tab);
  test_select_instrs_multiple_access(sym_tab);
  test_select_instrs_mv_instr(sym_tab);
  test_select_instrs_add_instr(sym_tab);
  test_select_instrs_mul_instr(sym_tab);
  test_select_instrs_div_instr(sym_tab);
  test_select_instrs_mod_instr(sym_tab);
  test_select_instrs_cmp(sym_tab);
  test_select_instrs_simple_branch(sym_tab);

  std::cout << "X86 Instruction Selection tests passed!" << std::endl;

  return 0;
}
