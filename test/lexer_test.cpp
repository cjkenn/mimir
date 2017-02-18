#include <assert.h>
#include <iostream>
#include "../src/token.h"
#include "../src/lexer.h"

// Test the Lex function on some single char symbols.
// The order of the assert statements matters here,
// because it must match the order of the symbols in the
// input file.
void test_lex_symbols() {
  std::string test_file = "lexer_input/symbol";
  Lexer lexer(test_file);

  Token paren_tkn = lexer.Lex();
  assert(paren_tkn.GetType() == TokenType::LEFT_PAREN_TKN);

  Token brace_tkn = lexer.Lex();
  assert(brace_tkn.GetType() == TokenType::LEFT_BRACE_TKN);

  Token plus_tkn = lexer.Lex();
  assert(plus_tkn.GetType() == TokenType::PLUS_TKN);

  Token minus_tkn = lexer.Lex();
  assert(minus_tkn.GetType() == TokenType::MINUS_TKN);

  Token equals_tkn = lexer.Lex();
  assert(equals_tkn.GetType() == TokenType::EQ_TKN);

  Token star_tkn = lexer.Lex();
  assert(star_tkn.GetType() == TokenType::STAR_TKN);

  Token slash_tkn = lexer.Lex();
  assert(slash_tkn.GetType() == TokenType::BACKSLASH_TKN);

  Token percent_tkn = lexer.Lex();
  assert(percent_tkn.GetType() == TokenType::PERCENT_TKN);

  Token excl_tkn = lexer.Lex();
  assert(excl_tkn.GetType() == TokenType::EXCL_TKN);

  Token lt_tkn = lexer.Lex();
  assert(lt_tkn.GetType() == TokenType::LT_TKN);

  Token gt_tkn = lexer.Lex();
  assert(gt_tkn.GetType() == TokenType::GT_TKN);

  Token lte_tkn = lexer.Lex();
  assert(lte_tkn.GetType() == TokenType::LTE_TKN);

  Token gte_tkn = lexer.Lex();
  assert(gte_tkn.GetType() == TokenType::GTE_TKN);

  Token eqeq_tkn = lexer.Lex();
  assert(eqeq_tkn.GetType() == TokenType::EQEQ_TKN);

  Token neq_tkn = lexer.Lex();
  assert(neq_tkn.GetType() == TokenType::NEQ_TKN);
}

// Test the Lex function on an identifier.
void test_lex_ident() {
  std::string test_file = "lexer_input/ident";
  Lexer lexer(test_file);

  Token ident_tkn = lexer.Lex();
  assert(ident_tkn.GetType() == TokenType::ID_TKN);
  assert(ident_tkn.GetText() == "ident1");
  assert(ident_tkn.GetLinePos() == 1);
  assert(ident_tkn.GetCharPos() == 1);
}

// Test the Lex function on a number.
void test_lex_num() {
  std::string test_file = "lexer_input/num";
  Lexer lexer(test_file);

  Token num_tkn = lexer.Lex();
  assert(num_tkn.GetType() == TokenType::NUM_TKN);
  assert(num_tkn.GetVal() == 111);
  assert(num_tkn.GetLinePos() == 1);
  assert(num_tkn.GetCharPos() == 1);
}

// Run all lexer tests in this file.
int main(int argc, char** argv) {
  test_lex_symbols();
  std::cout << "Lexer symbol tests passed!" << std::endl;

  test_lex_ident();
  std::cout << "Lexer ident tests passed!" << std::endl;

  test_lex_num();
  std::cout << "Lexer number tests passed!" << std::endl;

  return 0;
}
