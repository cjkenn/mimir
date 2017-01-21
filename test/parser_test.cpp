#include <assert.h>
#include <iostream>
#include <memory>
#include "../src/parser.h"
#include "../src/lexer.h"
#include "../src/ast.h"
#include "../src/token.h"

// Expected format:
//
//      Program
//         |
//     Expression
//         |
//        Set
//      /    \
//    Var   Term
//
// where var hold the variable name and term holds the value.
void test_parse_assign() {
  std::string test_file = "parser_input/simple_assign";
  auto lexer = std::make_shared<Lexer>(test_file);
  Parser parser(lexer);

  auto ast = parser.Parse();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto expr_ast = ast->GetChildren()[0];
  assert(expr_ast->GetType() == AstType::EXPR_AST);
  assert(expr_ast->GetChildren().size() == 1);

  auto set_ast = expr_ast->GetChildren()[0];
  assert(set_ast->GetType() == AstType::SET_AST);
  assert(set_ast->GetChildren().size() == 2);

  auto var_ast = set_ast->GetChildren()[0];
  assert(var_ast->GetType() == AstType::VAR_AST);
  assert(var_ast->GetChildren().size() == 0);
  assert(var_ast->GetText() == "x");

  auto term_ast = set_ast->GetChildren()[1];
  assert(term_ast->GetType() == AstType::CST_AST);
  assert(term_ast->GetVal() == 1);
}

void test_parse_assign_expression() {

}

void test_parse_if() {

}

void test_parse_if_else() {

}

void test_parse_while() {

}

int main(int argc, char** argv) {
  test_parse_assign();
  std::cout << "Parser Tests passed!" << std::endl;
  return 0;
}
