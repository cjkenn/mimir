#pragma once

#include "token.h"
#include "lexer.h"
#include "ast.h"

class Parser {
 public:
  Parser(Lexer lexer);
  ~Parser();
  Token GetTkn();
  void LogError(std::string error);
  AstNode Parse();

 private:
  Lexer lexer_;
  Token curr_tkn_;
};
