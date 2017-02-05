#pragma once

#include <memory>

#include "token.h"
#include "lexer.h"
#include "ast.h"

class Parser {
 public:
  Parser(std::shared_ptr<Lexer> lexer);
  ~Parser() {};
  void LogError(std::string error);
  AstNodePtr Parse();

 private:
  AstNodePtr Program();
  AstNodePtr Statement();
  AstNodePtr ParenExpr();
  AstNodePtr Expr();
  AstNodePtr Test();
  AstNodePtr Sum();
  AstNodePtr Term();
  void Expect(TokenType expected_type);
  void GetNextTkn();
  std::string PrettifyTokenType(TokenType type);

  std::shared_ptr<Lexer> lexer_;
  Token curr_tkn_;
};
