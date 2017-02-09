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

// Expected format:
//
//     If_Block
//        |
//      Block1
//        |
//      Block2
//
// Full input:
//
// if (x < 1) { y = 2; }
//
// Full output:
//
// start_:
// ld x, r0
// mv 1, r1
// cmp r0, r1
// jmpgt lbl0 --> new block begins right after this instr
// mv 2, r2
// sv r2
// jmp lbl0 --> new block begins right after this instr
// lbl0:
void test_gen_if_ast() {
  // TODO: Probably better to manually construct an ast rather
  // than parse one, since that adds a dependency to this test,
  // which should be focused only on the ir gen.
  // (ie. what if parsing breaks?)
  std::string test_file = "parser_input/if_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  Parser parser(lexer);

  auto ast = parser.Parse();
  IrGen ir;
  auto root_ir = ir.Gen(ast);

  assert(root_ir->GetLabel() == "start");
  assert(root_ir->GetInstructions().size() == 4);
  assert(root_ir->GetAdjacent().size() == 1);

  // assert we have the correct instructions
  // for the initial block.
  auto root_instrs = root_ir->GetInstructions();
  assert(root_instrs[0].GetType() == InstructionType::LD_INSTR);
  std::pair<std::string, std::string> args0("x", "r0");
  assert(root_instrs[0].GetArgs() == args0);

  assert(root_instrs[1].GetType() == InstructionType::MV_INSTR);
  std::pair<std::string, std::string> args1("1", "r1");
  assert(root_instrs[1].GetArgs() == args1);

  assert(root_instrs[2].GetType() == InstructionType::CMP_INSTR);
  std::pair<std::string, std::string> args2("r0", "r1");
  assert(root_instrs[2].GetArgs() == args2);

  assert(root_instrs[3].GetType() == InstructionType::JMPGT_INSTR);
  assert(root_instrs[3].GetDest() == "lbl0");

  // assert we have the correct instructions
  // for the second block.
  auto sec_ir = root_ir->GetAdjacent()[0];
  assert(sec_ir->GetInstructions().size() == 3);
  assert(sec_ir->GetAdjacent().size() == 1);

  auto sec_instrs = sec_ir->GetInstructions();
  assert(sec_instrs[0].GetType() == InstructionType::MV_INSTR);
  std::pair<std::string, std::string> s_args0("2", "r2");
  assert(sec_instrs[0].GetArgs() == s_args0);

  assert(sec_instrs[1].GetType() == InstructionType::SV_INSTR);
  assert(sec_instrs[1].GetDest() == "r2");

  assert(sec_instrs[2].GetType() == InstructionType::JMP_INSTR);
  assert(sec_instrs[2].GetDest() == "lbl0");

  // nothing in the third block for this test
  auto third_ir = sec_ir->GetAdjacent()[0];
  assert(third_ir->GetInstructions().size() == 0);
  assert(third_ir->GetLabel() == "lbl0");
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
// sv r2
// jmp lbl0 --> new block begins right after this instr
// lbl0:
// mv 3, r3
// sv r3
// jmp lbl1 --> new block begins right after this instr
// lbl1:
void test_gen_else_ast() {
  std::string test_file = "parser_input/if_else_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  Parser parser(lexer);

  auto ast = parser.Parse();
  IrGen ir;
  auto root_ir = ir.Gen(ast);

  assert(root_ir->GetLabel() == "start");
  assert(root_ir->GetInstructions().size() == 4);
  assert(root_ir->GetAdjacent().size() == 1);

  // assert we have the correct instructions
  // for the initial block.
  auto root_instrs = root_ir->GetInstructions();
  assert(root_instrs[0].GetType() == InstructionType::LD_INSTR);
  std::pair<std::string, std::string> args0("x", "r0");
  assert(root_instrs[0].GetArgs() == args0);

  assert(root_instrs[1].GetType() == InstructionType::MV_INSTR);
  std::pair<std::string, std::string> args1("1", "r1");
  assert(root_instrs[1].GetArgs() == args1);

  assert(root_instrs[2].GetType() == InstructionType::CMP_INSTR);
  std::pair<std::string, std::string> args2("r0", "r1");
  assert(root_instrs[2].GetArgs() == args2);

  assert(root_instrs[3].GetType() == InstructionType::JMPGT_INSTR);
  assert(root_instrs[3].GetDest() == "lbl0");

  // assert we have the correct instructions
  // for the second block.
  auto sec_ir = root_ir->GetAdjacent()[0];
  assert(sec_ir->GetInstructions().size() == 3);
  assert(sec_ir->GetAdjacent().size() == 1);

  auto sec_instrs = sec_ir->GetInstructions();
  assert(sec_instrs[0].GetType() == InstructionType::MV_INSTR);
  std::pair<std::string, std::string> s_args0("2", "r2");
  assert(sec_instrs[0].GetArgs() == s_args0);

  assert(sec_instrs[1].GetType() == InstructionType::SV_INSTR);
  assert(sec_instrs[1].GetDest() == "r2");

  assert(sec_instrs[2].GetType() == InstructionType::JMP_INSTR);
  assert(sec_instrs[2].GetDest() == "lbl0");

  // assert we have the correct instructions
  // for the third block.
  auto third_ir = sec_ir->GetAdjacent()[0];
  assert(third_ir->GetInstructions().size() == 3);
  assert(third_ir->GetLabel() == "lbl0");
  assert(third_ir->GetAdjacent().size() == 1);

  auto th_instrs = third_ir->GetInstructions();
  assert(th_instrs[0].GetType() == InstructionType::MV_INSTR);
  std::pair<std::string, std::string> t_args0("3", "r3");
  assert(th_instrs[0].GetArgs() == t_args0);

  assert(th_instrs[1].GetType() == InstructionType::SV_INSTR);
  assert(th_instrs[1].GetDest() == "r3");

  assert(th_instrs[2].GetType() == InstructionType::JMP_INSTR);
  // assert(th_instrs[2].GetDest() == "lbl1");

  // nothing in this next block
  auto fourth_ir = third_ir->GetAdjacent()[0];
  assert(fourth_ir->GetInstructions().size() == 0);
  assert(fourth_ir->GetAdjacent().size() == 0);
}

int main(int argc, char** argv) {
  test_gen_if_ast();
  test_gen_else_ast();

  std::cout << "Ir gen tests passed!" << std::endl;
  return 0;
}
