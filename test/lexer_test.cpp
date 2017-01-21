#include <assert.h>
#include <iostream>
#include "../src/token.h"
#include "../src/lexer.h"

// Test the Lex function on some single char symbols.
// We don't test all symbols exhaustively, but this
// sample of single chars should suffice.
void test_lex_symbols() {
  std::string test_file = "lexerSymbolInput";
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
  assert(equals_tkn.GetType() == TokenType::EQUALS_TKN);
}

// Test the Lex function on an identifier.
void test_lex_ident() {
  std::string test_file = "lexerIdentInput";
  Lexer lexer(test_file);

  Token ident_tkn = lexer.Lex();
  assert(ident_tkn.GetType() == TokenType::ID_TKN);
  assert(ident_tkn.GetText() == "ident1");
}

// Test the Lex function on a number.
void test_lex_num() {
  std::string test_file = "lexerNumInput";
  Lexer lexer(test_file);

  Token num_tkn = lexer.Lex();
  assert(num_tkn.GetType() == TokenType::NUM_TKN);
  assert(num_tkn.GetVal() == 111);
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
