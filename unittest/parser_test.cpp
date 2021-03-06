#include <assert.h>
#include <iostream>
#include <memory>
#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "token.h"
#include "symbol_table.h"
#include "error.h"

using namespace mimir;

Parser get_parser_for_file(std::string filename) {
  std::string test_file = "parser_input/" + filename;
  auto lexer = std::make_shared<Lexer>(test_file);
  auto sym_tab = std::make_shared<SymbolTable>();
  Parser parser(lexer, sym_tab);

  return parser;
}

// Expected format:
//
//      Program
//         |
//        Expr
//         |
//        Set
//      _/   \_
//    Var  Term
//
// where var holds the variable name and term holds the value.
void test_parse_assign() {
  auto parser = get_parser_for_file("simple_assign");
  auto ast = parser.Parse().GetAst();

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
  auto parser = get_parser_for_file("expr_assign");
  auto ast = parser.Parse().GetAst();

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
//        Expr
//         |
//        Mul
//      _/  \_
//    Num    Num
void test_parse_mul_expr() {
  auto parser = get_parser_for_file("mul_expr");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto expr_ast = ast->GetChildren()[0];
  assert(expr_ast->GetType() == AstType::EXPR_AST);
  assert(expr_ast->GetChildren().size() == 1);

  auto mul_ast = expr_ast->GetChildren()[0];
  assert(mul_ast->GetType() == AstType::MUL_AST);
  assert(mul_ast->GetChildren().size() == 2);

  auto first_term_ast = mul_ast->GetChildren()[0];
  assert(first_term_ast->GetType() == AstType::CST_AST);
  assert(first_term_ast->GetVal() == 5);

  auto second_term_ast = mul_ast->GetChildren()[1];
  assert(second_term_ast->GetType() == AstType::CST_AST);
  assert(second_term_ast->GetVal() == 6);
}

// Expected format:
//
//      Program
//         |
//        Expr
//         |
//        Div
//      _/  \_
//    Num    Num
void test_parse_div_expr() {
  auto parser = get_parser_for_file("div_expr");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto expr_ast = ast->GetChildren()[0];
  assert(expr_ast->GetType() == AstType::EXPR_AST);
  assert(expr_ast->GetChildren().size() == 1);

  auto mul_ast = expr_ast->GetChildren()[0];
  assert(mul_ast->GetType() == AstType::DIV_AST);
  assert(mul_ast->GetChildren().size() == 2);

  auto first_term_ast = mul_ast->GetChildren()[0];
  assert(first_term_ast->GetType() == AstType::CST_AST);
  assert(first_term_ast->GetVal() == 5);

  auto second_term_ast = mul_ast->GetChildren()[1];
  assert(second_term_ast->GetType() == AstType::CST_AST);
  assert(second_term_ast->GetVal() == 6);
}

// Expected format:
//
//      Program
//         |
//      __If_____
//    _/         \_
//  Expr(LTE)    ...
//  _/   \_
// Num  Num
//
void test_parse_lte_test() {
  auto parser = get_parser_for_file("lte_test");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto if_ast = ast->GetChildren()[0];
  assert(if_ast->GetType() == AstType::IF_AST);
  assert(if_ast->GetChildren().size() == 2);

  auto lte_expr_ast = if_ast->GetChildren()[0];
  assert(lte_expr_ast->GetType() == AstType::LTE_AST);
  assert(lte_expr_ast->GetChildren().size() == 2);

  auto lte_expr_term = lte_expr_ast->GetChildren()[0];
  assert(lte_expr_term->GetType() == AstType::CST_AST);
  assert(lte_expr_term->GetChildren().size() == 0);
  assert(lte_expr_term->GetVal() == 5);

  auto lte_expr_test = lte_expr_ast->GetChildren()[1];
  assert(lte_expr_test->GetType() == AstType::CST_AST);
  assert(lte_expr_test->GetChildren().size() == 0);
  assert(lte_expr_test->GetVal() == 7);
}

// Expected format:
//
//      Program
//         |
//      __If_____
//    _/         \_
//  Expr(EQEQ)   ...
//  _/   \_
// Num  Num
//
void test_parse_eqeq_test() {
  auto parser = get_parser_for_file("eqeq_test");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto if_ast = ast->GetChildren()[0];
  assert(if_ast->GetType() == AstType::IF_AST);
  assert(if_ast->GetChildren().size() == 2);

  auto eqeq_expr_ast = if_ast->GetChildren()[0];
  assert(eqeq_expr_ast->GetType() == AstType::EQEQ_AST);
  assert(eqeq_expr_ast->GetChildren().size() == 2);

  auto eqeq_expr_term = eqeq_expr_ast->GetChildren()[0];
  assert(eqeq_expr_term->GetType() == AstType::CST_AST);
  assert(eqeq_expr_term->GetChildren().size() == 0);
  assert(eqeq_expr_term->GetVal() == 5);

  auto eqeq_expr_test = eqeq_expr_ast->GetChildren()[1];
  assert(eqeq_expr_test->GetType() == AstType::CST_AST);
  assert(eqeq_expr_test->GetChildren().size() == 0);
  assert(eqeq_expr_test->GetVal() == 7);
}

// Expected format:
//
//      Program
//         |
//      __If_____
//    _/         \_
//  Expr(NEQ)   ...
//  _/   \_
// Num  Num
//
void test_parse_neq_test() {
  auto parser = get_parser_for_file("neq_test");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto if_ast = ast->GetChildren()[0];
  assert(if_ast->GetType() == AstType::IF_AST);
  assert(if_ast->GetChildren().size() == 2);

  auto eqeq_expr_ast = if_ast->GetChildren()[0];
  assert(eqeq_expr_ast->GetType() == AstType::NEQ_AST);
  assert(eqeq_expr_ast->GetChildren().size() == 2);

  auto eqeq_expr_term = eqeq_expr_ast->GetChildren()[0];
  assert(eqeq_expr_term->GetType() == AstType::CST_AST);
  assert(eqeq_expr_term->GetChildren().size() == 0);
  assert(eqeq_expr_term->GetVal() == 5);

  auto eqeq_expr_test = eqeq_expr_ast->GetChildren()[1];
  assert(eqeq_expr_test->GetType() == AstType::CST_AST);
  assert(eqeq_expr_test->GetChildren().size() == 0);
  assert(eqeq_expr_test->GetVal() == 7);
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
void test_parse_if() {
  auto parser = get_parser_for_file("if_statement");
  auto ast = parser.Parse().GetAst();

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

// Expected format:
//
//         Program
//            |
//     ______Else______
//   _/       |        \_
//  Expr(LT) Seq       Seq
//   _/ \_    |         |
// Var  Num  Expr      Expr
//            |         |
//           Set       Set
//         _/  \_     _/ \_
//        Var  Num   Var  Num
void test_parse_if_else() {
  auto parser = get_parser_for_file("if_else_statement");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto else_ast = ast->GetChildren()[0];
  assert(else_ast->GetType() == AstType::ELSE_AST);
  assert(else_ast->GetChildren().size() == 3);

  // Check the expression nodes
  auto lt_expr_ast = else_ast->GetChildren()[0];
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

  // Check the original if sequence
  auto seq_ast = else_ast->GetChildren()[1];
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

  // Check the else sequence, entered if the original condition is false
  auto else_seq_ast = else_ast->GetChildren()[2];
  assert(else_seq_ast->GetType() == AstType::SEQ_AST);
  assert(else_seq_ast->GetChildren().size() == 1);

  auto else_seq_expr_ast = else_seq_ast->GetChildren()[0];
  assert(else_seq_expr_ast->GetType() == AstType::EXPR_AST);
  assert(else_seq_expr_ast->GetChildren().size() == 1);

  auto else_set_expr_ast = else_seq_expr_ast->GetChildren()[0];
  assert(else_set_expr_ast->GetType() == AstType::SET_AST);
  assert(else_set_expr_ast->GetChildren().size() == 2);

  auto else_set_var_ast = else_set_expr_ast->GetChildren()[0];
  assert(else_set_var_ast->GetText() == "y");

  auto else_set_val_ast = else_set_expr_ast->GetChildren()[1];
  assert(else_set_val_ast->GetVal() == 3);
}

// Expected format:
//
//         Program
//            |
//     _____While____
//   _/              \_
//  Expr(LT)         Seq
//   _/ \_            |
// Var  Num          Expr
//                    |
//                   Set
//                 _/  \_
//                Var   Op
//                    _/ \_
//                   Var Num
void test_parse_while() {
  auto parser = get_parser_for_file("while");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto while_ast = ast->GetChildren()[0];
  assert(while_ast->GetType() == AstType::WHILE_AST);
  assert(while_ast->GetChildren().size() == 2);

  // Check the expression nodes
  auto lt_expr_ast = while_ast->GetChildren()[0];
  assert(lt_expr_ast->GetType() == AstType::LT_AST);
  assert(lt_expr_ast->GetChildren().size() == 2);

  auto lt_expr_term = lt_expr_ast->GetChildren()[0];
  assert(lt_expr_term->GetType() == AstType::VAR_AST);
  assert(lt_expr_term->GetChildren().size() == 0);
  assert(lt_expr_term->GetText() == "x");

  auto lt_expr_test = lt_expr_ast->GetChildren()[1];
  assert(lt_expr_test->GetType() == AstType::CST_AST);
  assert(lt_expr_test->GetChildren().size() == 0);
  assert(lt_expr_test->GetVal() == 10);

  // Check the sequence inside the while loop
  auto seq_ast = while_ast->GetChildren()[1];
  assert(seq_ast->GetType() == AstType::SEQ_AST);
  assert(seq_ast->GetChildren().size() == 1);

  auto seq_expr_ast = seq_ast->GetChildren()[0];
  assert(seq_expr_ast->GetType() == AstType::EXPR_AST);
  assert(seq_expr_ast->GetChildren().size() == 1);

  auto set_expr_ast = seq_expr_ast->GetChildren()[0];
  assert(set_expr_ast->GetType() == AstType::SET_AST);
  assert(set_expr_ast->GetChildren().size() == 2);

  auto set_var_ast = set_expr_ast->GetChildren()[0];
  assert(set_var_ast->GetText() == "x");

  auto next_set_ast = set_expr_ast->GetChildren()[1];
  assert(next_set_ast->GetType() == AstType::ADD_AST);
  assert(next_set_ast->GetChildren().size() == 2);
}

// Expected format:
//
//       ___Program___
//     _/             \_
//    Expr            Expr
//     |               |
//    Set             Set
//  _/  \_          _/  \_
// Var  Num        Var  Num
void test_parse_two_statements() {
  auto parser = get_parser_for_file("two_statements");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 2);

  auto first_expr_ast = ast->GetChildren()[0];
  assert(first_expr_ast->GetType() == AstType::EXPR_AST);
  assert(first_expr_ast->GetChildren().size() == 1);

  auto second_expr_ast = ast->GetChildren()[1];
  assert(second_expr_ast->GetType() == AstType::EXPR_AST);
  assert(second_expr_ast->GetChildren().size() == 1);

  auto first_set_ast = first_expr_ast->GetChildren()[0];
  assert(first_set_ast->GetType() == AstType::SET_AST);
  assert(first_set_ast->GetChildren().size() == 2);

  auto second_set_ast = second_expr_ast->GetChildren()[0];
  assert(second_set_ast->GetType() == AstType::SET_AST);
  assert(second_set_ast->GetChildren().size() == 2);

  auto first_var_ast = first_set_ast->GetChildren()[0];
  assert(first_var_ast->GetType() == AstType::VAR_AST);
  assert(first_var_ast->GetText() == "x");

  auto first_num_ast = first_set_ast->GetChildren()[1];
  assert(first_num_ast->GetType() == AstType::CST_AST);
  assert(first_num_ast->GetVal() == 1);

  auto second_var_ast = second_set_ast->GetChildren()[0];
  assert(second_var_ast->GetType() == AstType::VAR_AST);
  assert(second_var_ast->GetText() == "y");

  auto second_num_ast = second_set_ast->GetChildren()[1];
  assert(second_num_ast->GetType() == AstType::CST_AST);
  assert(second_num_ast->GetVal() == 2);
}

// Expected format:
//
//    Program
//      |
//     Expr
//      |
//     Set
//   _/  \_
//  Var  Neg
//        |
//       Var
void test_parse_negation_op() {
  auto parser = get_parser_for_file("negation");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto expr_ast = ast->GetChildren()[0];
  assert(expr_ast->GetType() == AstType::EXPR_AST);
  assert(expr_ast->GetChildren().size() == 1);

  auto set_ast = expr_ast->GetChildren()[0];
  assert(set_ast->GetType() == AstType::SET_AST);
  assert(set_ast->GetChildren().size() == 2);

  auto first_var_ast = set_ast->GetChildren()[0];
  assert(first_var_ast->GetType() == AstType::VAR_AST);
  assert(first_var_ast->GetText() == "x");

  auto neg_ast = set_ast->GetChildren()[1];
  assert(neg_ast->GetType() == AstType::NEG_AST);
  assert(neg_ast->GetChildren().size() == 1);

  auto second_var_ast = neg_ast->GetChildren()[0];
  assert(second_var_ast->GetType() == AstType::VAR_AST);
  assert(second_var_ast->GetText() == "y");
}

// Expected format:
//
//       Program
//          |
//       Function
//       _/     \_
//    Params   Statement
//    _/  \_       |
//  Var    Var    Set
//                 |
//                ...
void test_parse_func_def() {
  auto parser = get_parser_for_file("func_statement");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto func_ast = ast->GetChildren()[0];
  assert(func_ast->GetType() == AstType::FUNC_AST);
  assert(func_ast->GetChildren().size() == 2);

  auto params_ast = func_ast->GetChildren()[0];
  assert(params_ast->GetType() == AstType::PARAMS_AST);
  assert(params_ast->GetChildren().size() == 2);
  assert(params_ast->GetChildren()[0]->GetText() == "x");
  assert(params_ast->GetChildren()[1]->GetText() == "y");

  auto statement_ast = func_ast->GetChildren()[1];
  assert(statement_ast->GetType() == AstType::SEQ_AST);
  assert(statement_ast->GetChildren().size() == 1);
}

// Expected format:
//
//   Program
//      |
//   FuncCall
//      |
//    Params
//    _/  \_
//  Var    Var
void test_parse_func_call() {
  auto parser = get_parser_for_file("func_call");
  auto ast = parser.Parse().GetAst();

  assert(ast->GetType() == AstType::PROG_AST);
  assert(ast->GetChildren().size() == 1);

  auto func_ast = ast->GetChildren()[0];
  assert(func_ast->GetType() == AstType::FUNC_CALL_AST);
  assert(func_ast->GetChildren().size() == 1);

  auto params_ast = func_ast->GetChildren()[0];
  assert(params_ast->GetType() == AstType::PARAMS_AST);
  assert(params_ast->GetChildren().size() == 2);
  assert(params_ast->GetChildren()[0]->GetText() == "x");
  assert(params_ast->GetChildren()[1]->GetText() == "y");
}

int main(int argc, char** argv) {
  test_parse_assign();
  test_parse_assign_expression();
  test_parse_mul_expr();
  test_parse_div_expr();
  test_parse_lte_test();
  test_parse_eqeq_test();
  test_parse_neq_test();
  test_parse_if();
  test_parse_if_else();
  test_parse_while();
  test_parse_two_statements();
  test_parse_negation_op();
  test_parse_func_def();
  test_parse_func_call();

  std::cout << "Parser Tests passed!" << std::endl;
  return 0;
}
