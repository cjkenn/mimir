#include <assert.h>
#include <iostream>
#include <memory>
#include "ir_gen.h"
#include "ir_instr.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "symbol_table.h"

using namespace mimir;

// Input:
//
// if (x < 1) { y = 2; }
//
// Expected Output:
//
// lbl0:
// ld x, r0
// mv 1, r1
// cmp r0, r1
// jmpgt lbl1
// mv 2, r2
// sv r2, y
// lbl1:
void test_gen_if_ast() {
  // TODO: Probably better to manually construct an ast rather
  // than parse one, since that adds a dependency to this test,
  // which should be focused only on the ir gen.
  // (ie. what if parsing breaks?)
  std::string test_file = "parser_input/if_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  auto sym_tab = std::make_shared<SymbolTable>();
  Parser parser(lexer, sym_tab);

  auto ast = parser.Parse().GetAst();
  IrGen ir_gen;
  std::vector<IrInstrPtr> ir = ir_gen.Gen(ast);

  assert(ir.size() == 7);

  // assert we have the correct instructions
  assert(ir[0]->GetType() == IrInstrType::LD_INSTR);
  assert(ir[0]->GetLabel() == "lbl0");
  assert(ir[0]->GetArgs().first == "x");
  assert(ir[0]->GetArgs().second == "r0");
  assert(ir[0]->GetDest() == "r0");

  assert(ir[1]->GetType() == IrInstrType::MV_INSTR);
  assert(ir[1]->GetLabel() == "lbl0");
  assert(ir[1]->GetArgs().first == "1");
  assert(ir[1]->GetArgs().second == "r1");
  assert(ir[1]->GetDest() == "r1");

  assert(ir[2]->GetType() == IrInstrType::CMP_INSTR);
  assert(ir[2]->GetLabel() == "lbl0");
  assert(ir[2]->GetArgs().first == "r0");
  assert(ir[2]->GetArgs().second == "r1");
  assert(ir[2]->GetDest() == "r0");

  assert(ir[3]->GetType() == IrInstrType::JMPGT_INSTR);
  assert(ir[3]->GetLabel() == "lbl0");
  assert(ir[3]->GetDest() == "lbl1");

  assert(ir[4]->GetType() == IrInstrType::MV_INSTR);
  assert(ir[4]->GetLabel() == "lbl0");
  assert(ir[4]->GetArgs().first == "2");
  assert(ir[4]->GetArgs().second == "r2");
  assert(ir[4]->GetDest() == "r2");

  assert(ir[5]->GetType() == IrInstrType::SV_INSTR);
  assert(ir[5]->GetLabel() == "lbl0");
  assert(ir[5]->GetArgs().first == "r2");
  assert(ir[5]->GetArgs().second == "y");
  assert(ir[5]->GetDest() == "y");

  assert(ir[6]->GetType() == IrInstrType::NOP_INSTR);
  assert(ir[6]->GetLabel() == "lbl1");
}

// Input:
//
// if (x < 1) { y = 2; } else { y = 3; }
//
// Expected Output:
//
// lbl0:
// ld x, r0
// mv 1, r1
// cmp r0, r1
// jmpgt lbl1
// mv 2, r2
// sv r2, y
// lbl1:
// mv 3, r3
// sv r3, y
// lbl2:
void test_gen_else_ast() {
  std::string test_file = "parser_input/if_else_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  auto sym_tab = std::make_shared<SymbolTable>();
  Parser parser(lexer, sym_tab);

  auto ast = parser.Parse().GetAst();
  IrGen ir_gen;
  std::vector<IrInstrPtr> ir = ir_gen.Gen(ast);

  assert(ir.size() == 9);

  // assert we have the correct instructions
  assert(ir[0]->GetType() == IrInstrType::LD_INSTR);
  assert(ir[0]->GetLabel() == "lbl0");
  assert(ir[0]->GetArgs().first == "x");
  assert(ir[0]->GetArgs().second == "r0");
  assert(ir[0]->GetDest() == "r0");

  assert(ir[1]->GetType() == IrInstrType::MV_INSTR);
  assert(ir[1]->GetLabel() == "lbl0");
  assert(ir[1]->GetArgs().first == "1");
  assert(ir[1]->GetArgs().second == "r1");
  assert(ir[1]->GetDest() == "r1");

  assert(ir[2]->GetType() == IrInstrType::CMP_INSTR);
  assert(ir[2]->GetLabel() == "lbl0");
  assert(ir[2]->GetArgs().first == "r0");
  assert(ir[2]->GetArgs().second == "r1");
  assert(ir[2]->GetDest() == "r0");

  assert(ir[3]->GetType() == IrInstrType::JMPGT_INSTR);
  assert(ir[3]->GetLabel() == "lbl0");
  assert(ir[3]->GetDest() == "lbl1");

  assert(ir[4]->GetType() == IrInstrType::MV_INSTR);
  assert(ir[4]->GetLabel() == "lbl0");
  assert(ir[4]->GetArgs().first == "2");
  assert(ir[4]->GetArgs().second == "r2");
  assert(ir[4]->GetDest() == "r2");

  assert(ir[5]->GetType() == IrInstrType::SV_INSTR);
  assert(ir[5]->GetLabel() == "lbl0");
  assert(ir[5]->GetArgs().first == "r2");
  assert(ir[5]->GetArgs().second == "y");
  assert(ir[5]->GetDest() == "y");

  assert(ir[6]->GetType() == IrInstrType::MV_INSTR);
  assert(ir[6]->GetLabel() == "lbl1");
  assert(ir[6]->GetArgs().first == "3");
  assert(ir[6]->GetArgs().second == "r3");
  assert(ir[6]->GetDest() == "r3");

  assert(ir[7]->GetType() == IrInstrType::SV_INSTR);
  assert(ir[7]->GetLabel() == "lbl1");
  assert(ir[7]->GetArgs().first == "r3");
  assert(ir[7]->GetArgs().second == "y");
  assert(ir[7]->GetDest() == "y");

  assert(ir[8]->GetType() == IrInstrType::NOP_INSTR);
  assert(ir[8]->GetLabel() == "lbl1");
}

// Input:
//
// while (x < 10) { x = x + 1; }
//
// Expected Output:
//
// lbl0:
// ld x, r0
// mv 10, r1
// cmp r0, r1
// jmpgt lbl1
// ld x, r2
// mv 1, r3
// add r2, r3
// sv r3, x
// jmp lbl0
// lbl1:
void test_gen_while_ast() {
  std::string test_file = "parser_input/while";
  auto lexer = std::make_shared<Lexer>(test_file);
  auto sym_tab = std::make_shared<SymbolTable>();
  Parser parser(lexer, sym_tab);

  auto ast = parser.Parse().GetAst();
  IrGen ir_gen;
  std::vector<IrInstrPtr> ir = ir_gen.Gen(ast);

  assert(ir.size() == 10);

  // assert we have the correct instructions
  assert(ir[0]->GetType() == IrInstrType::LD_INSTR);
  assert(ir[0]->GetLabel() == "lbl0");
  assert(ir[0]->GetArgs().first == "x");
  assert(ir[0]->GetArgs().second == "r0");
  assert(ir[0]->GetDest() == "r0");

  assert(ir[1]->GetType() == IrInstrType::MV_INSTR);
  assert(ir[1]->GetLabel() == "lbl0");
  assert(ir[1]->GetArgs().first == "10");
  assert(ir[1]->GetArgs().second == "r1");
  assert(ir[1]->GetDest() == "r1");

  assert(ir[2]->GetType() == IrInstrType::CMP_INSTR);
  assert(ir[2]->GetLabel() == "lbl0");
  assert(ir[2]->GetArgs().first == "r0");
  assert(ir[2]->GetArgs().second == "r1");
  assert(ir[2]->GetDest() == "r0");

  assert(ir[3]->GetType() == IrInstrType::JMPGT_INSTR);
  assert(ir[3]->GetLabel() == "lbl0");
  assert(ir[3]->GetDest() == "lbl1");

  assert(ir[4]->GetType() == IrInstrType::LD_INSTR);
  assert(ir[4]->GetLabel() == "lbl0");
  assert(ir[4]->GetArgs().first == "x");
  assert(ir[4]->GetArgs().second == "r2");
  assert(ir[4]->GetDest() == "r2");

  assert(ir[5]->GetType() == IrInstrType::MV_INSTR);
  assert(ir[5]->GetLabel() == "lbl0");
  assert(ir[5]->GetArgs().first == "1");
  assert(ir[5]->GetArgs().second == "r3");
  assert(ir[5]->GetDest() == "r3");

  assert(ir[6]->GetType() == IrInstrType::ADD_INSTR);
  assert(ir[6]->GetLabel() == "lbl0");
  assert(ir[6]->GetArgs().first == "r2");
  assert(ir[6]->GetArgs().second == "r3");
  assert(ir[6]->GetDest() == "r3");

  assert(ir[7]->GetType() == IrInstrType::SV_INSTR);
  assert(ir[7]->GetLabel() == "lbl0");
  assert(ir[7]->GetArgs().first == "r3");
  assert(ir[7]->GetArgs().second == "x");
  assert(ir[7]->GetDest() == "x");

  assert(ir[8]->GetType() == IrInstrType::JMP_INSTR);
  assert(ir[8]->GetDest() == "lbl0");

  assert(ir[9]->GetType() == IrInstrType::NOP_INSTR);
  assert(ir[9]->GetLabel() == "lbl1");
}

int main(int argc, char** argv) {
  test_gen_if_ast();
  test_gen_else_ast();
  test_gen_while_ast();

  std::cout << "Ir gen tests passed!" << std::endl;
  return 0;
}
