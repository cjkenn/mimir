#pragma once

#include "token.h"
#include "lexer.h"
#include "ast/expr.h"

class Parser {
 public:
  Parser(Lexer lexer) : m_lexer(lexer) {};
  ~Parser() {};
  Token get_tkn();
  void log_error(std::string error);
  ExprAST parse();
 private:
  Lexer m_lexer;
  Token curr_tkn;
};
