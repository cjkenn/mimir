#pragma once

#include <memory>
#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "symbol_table.h"

class Parser {
 public:
  Parser(std::shared_ptr<Lexer> lexer, std::shared_ptr<SymbolTable> sym_tab);
  ~Parser() {};
  void LogError(std::string error);
  AstNodePtr Parse();

 private:
  AstNodePtr Program();
  AstNodePtr Statement();
  AstNodePtr ParenExpr();
  AstNodePtr Expr();
  AstNodePtr Test();
  AstNodePtr BinOp();
  AstNodePtr Term();

  void Expect(TokenType expected_type);
  void GetNextTkn();
  bool CurrTknIsBinOpTkn();
  AstType GetBinOpAstFromTkn();
  bool CurrTknIsTestTkn();
  AstType GetTestAstFromTkn();
  std::string PrettifyTokenType(TokenType type);

  std::shared_ptr<Lexer> lexer_;
  Token curr_tkn_;
  std::shared_ptr<SymbolTable> sym_tab_;
};
