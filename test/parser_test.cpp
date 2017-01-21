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
//      _/   \_
//    Var  Term
//
// where var holds the variable name and term holds the value.
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

// Expected format:
//
//      Program
//         |
//     Expression
//         |
//        Set
//      _/  \_
//     Var   Op
//         _/  \_
//        Num  Num
//
// where var holds the variable name and op contains the terms
// in the addition operation
void test_parse_assign_expression() {
  std::string test_file = "parser_input/expr_assign";
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

  auto add_ast = set_ast->GetChildren()[1];
  assert(add_ast->GetType() == AstType::ADD_AST);
  assert(add_ast->GetChildren().size() == 2);

  auto first_term_ast = add_ast->GetChildren()[0];
  assert(first_term_ast->GetType() == AstType::CST_AST);
  assert(first_term_ast->GetVal() == 5);

  auto second_term_ast = add_ast->GetChildren()[1];
  assert(second_term_ast->GetType() == AstType::CST_AST);
  assert(second_term_ast->GetVal() == 2);
}

// Expected format:
//
//      Program
//         |
//      __If_____
//    _/         \_
//  Expr(LT)     Sequence
//  _/   \_         |
// Var  Num       Expr
//                 |
//                Set
//              _/  \_
//             Var   Num
//
void test_parse_if() {
  std::string test_file = "parser_input/if_statement";
  auto lexer = std::make_shared<Lexer>(test_file);
  Parser parser(lexer);

  auto ast = parser.Parse();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto if_ast = ast->GetChildren()[0];
  assert(if_ast->GetType() == AstType::IF_AST);
  assert(if_ast->GetChildren().size() == 2);

  auto lt_expr_ast = if_ast->GetChildren()[0];
  assert(lt_expr_ast->GetType() == AstType::LT_AST);
  assert(lt_expr_ast->GetChildren().size() == 2);

  auto lt_expr_term = lt_expr_ast->GetChildren()[0];
  assert(lt_expr_term->GetType() == AstType::VAR_AST);
  assert(lt_expr_term->GetChildren().size() == 0);
  assert(lt_expr_term->GetText() == "x");

  auto lt_expr_test = lt_expr_ast->GetChildren()[1];
  assert(lt_expr_test->GetType() == AstType::CST_AST);
  assert(lt_expr_test->GetChildren().size() == 0);
  assert(lt_expr_test->GetVal() == 1);

  auto seq_ast = if_ast->GetChildren()[1];
  assert(seq_ast->GetType() == AstType::SEQ_AST);
  assert(seq_ast->GetChildren().size() == 1);

  auto seq_expr_ast = seq_ast->GetChildren()[0];
  assert(seq_expr_ast->GetType() == AstType::EXPR_AST);
  assert(seq_expr_ast->GetChildren().size() == 1);

  auto set_expr_ast = seq_expr_ast->GetChildren()[0];
  assert(set_expr_ast->GetType() == AstType::SET_AST);
  assert(set_expr_ast->GetChildren().size() == 2);

  auto set_var_ast = set_expr_ast->GetChildren()[0];
  assert(set_var_ast->GetText() == "y");

  auto set_val_ast = set_expr_ast->GetChildren()[1];
  assert(set_val_ast->GetVal() == 2);
}

void test_parse_if_else() {

}

void test_parse_while() {

}

int main(int argc, char** argv) {
  test_parse_assign();
  test_parse_assign_expression();
  test_parse_if();

  std::cout << "Parser Tests passed!" << std::endl;
  return 0;
}
