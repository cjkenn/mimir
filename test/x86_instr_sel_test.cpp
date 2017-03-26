#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../src/x86_instr_sel.h"
#include "../src/symbol_table.h"
#include "../src/symbol.h"
#include "../src/ir_instr.h"
#include "../src/x86_instr.h"
#include "../src/cfg_node.h"
#include "../src/ast.h"

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

  X86InstrSel x86is(sym_tab);
  x86is.SelectInstrs(cfg_node);

  assert(cfg_node->GetX86Instrs().size() == 1);

  auto first_x86 = cfg_node->GetX86Instrs()[0];
  assert(first_x86->GetType() == X86InstrType::MOV_X86);
  assert(first_x86->GetFirstArg() == "[ebp-4]");
  assert(first_x86->GetSecondArg() == "10");
}


int main(int argc, char **argv) {
  auto sym_tab = std::make_shared<SymbolTable>();
  auto x_node = std::make_shared<AstNode>(AstType::VAR_AST);
  x_node->SetText("x");

  sym_tab->Insert(x_node);

  test_select_instrs_simple_assign(sym_tab);

  std::cout << "X86 Instruction Selection tests passed!" << std::endl;

  return 0;
}
