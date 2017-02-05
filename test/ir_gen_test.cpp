#include <assert.h>
#include <iostream>
#include <memory>
#include "../src/ir_block.h"
#include "../src/ir_gen.h"
#include "../src/ast.h"
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/token.h"

// Expected format:
//     If_Block ----
//        |        |
//      Block1     |
//        |        |
//      BLock2 <---|
void test_gen_if_ast() {
  std::string test_file = "parser_input/if_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  Parser parser(lexer);

  auto ast = parser.Parse();
  IrGen ir;
  auto blocks = ir.Gen(ast);
}

int main(int argc, char** argv) {
  test_gen_if_ast();

  std::cout << "Ir gen tests passed!" << std::endl;
  return 0;
}
