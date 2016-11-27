#pragma once

#include "token.h"
#include "lexer.h"
#include "ast.h"

class Parser {
 public:
  Parser(Lexer *lexer);
  ~Parser();
  void LogError(std::string error);
  AstNode *Parse();

 private:
  AstNode *Program();
  AstNode *Statement();
  AstNode *ParenExpr();
  AstNode *Expr();
  AstNode *Test();
  AstNode *Sum();
  AstNode *Term();
  void Expect(TokenType expected_type);
  void GetNextTkn();

  Lexer *lexer_;
  Token curr_tkn_;
};
