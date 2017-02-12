#include <assert.h>
#include <iostream>
#include <memory>
#include "../src/ir_block.h"
#include "../src/ir_gen.h"
#include "../src/instruction.h"
#include "../src/ast.h"
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/token.h"

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
// sv r3
// lbl1:
void test_gen_if_ast() {
  // TODO: Probably better to manually construct an ast rather
  // than parse one, since that adds a dependency to this test,
  // which should be focused only on the ir gen.
  // (ie. what if parsing breaks?)
  std::string test_file = "parser_input/if_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  Parser parser(lexer);

  auto ast = parser.Parse();
  IrGen ir_gen;
  std::vector<InstrPtr> ir = ir_gen.Gen(ast);

  assert(ir.size() == 6);

  // assert we have the correct instructions
  assert(ir[0]->GetType() == InstructionType::LD_INSTR);
  assert(ir[0]->GetLabel() == "lbl0");
  assert(ir[0]->GetArgs().first == "x");
  assert(ir[0]->GetArgs().second == "r0");
  assert(ir[0]->GetDest() == "r0");

  assert(ir[1]->GetType() == InstructionType::MV_INSTR);
  assert(ir[1]->GetLabel() == "lbl0");
  assert(ir[1]->GetArgs().first == "1");
  assert(ir[1]->GetArgs().second == "r1");
  assert(ir[1]->GetDest() == "r1");

  assert(ir[2]->GetType() == InstructionType::CMP_INSTR);
  assert(ir[2]->GetLabel() == "lbl0");
  assert(ir[2]->GetArgs().first == "r0");
  assert(ir[2]->GetArgs().second == "r1");
  assert(ir[2]->GetDest() == "r0");

  assert(ir[3]->GetType() == InstructionType::JMPGT_INSTR);
  assert(ir[3]->GetLabel() == "lbl0");
  assert(ir[3]->GetDest() == "lbl1");

  assert(ir[4]->GetType() == InstructionType::MV_INSTR);
  assert(ir[4]->GetLabel() == "lbl0");
  assert(ir[4]->GetArgs().first == "2");
  assert(ir[4]->GetArgs().second == "r2");
  assert(ir[4]->GetDest() == "r2");

  assert(ir[5]->GetType() == InstructionType::SV_INSTR);
  assert(ir[5]->GetLabel() == "lbl0");
  assert(ir[5]->GetDest() == "r3");
}

// Expected format:
//
//     If_Block
//        |
//      Block1
//        |
//      Block2
//        |
//      Block3
//
// Full input:
//
// if (x < 1) { y = 2; } else { y = 3; }
//
// Full output:
//
// start:
// ld x, r0
// mv 1, r1
// cmp r0, r1
// jmpgt lbl0 --> new block begins right after this instr
// mv 2, r2
// sv r2 --> new block begins right after this instr
// lbl0:
// mv 3, r3
// sv r3  --> new block begins right after this instr
// lbl1:
void test_gen_else_ast() {
  std::string test_file = "parser_input/if_else_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  Parser parser(lexer);

  auto ast = parser.Parse();
  IrGen ir;
  auto root_ir = ir.Gen(ast);
}

int main(int argc, char** argv) {
  test_gen_if_ast();
  // test_gen_else_ast();

  std::cout << "Ir gen tests passed!" << std::endl;
  return 0;
}
