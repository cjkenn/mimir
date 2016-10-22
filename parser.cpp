#include <string>
#include <iostream>
#include "token.h"
#include "parser.h"
#include "ast.h"

using namespace std;

AstNode Parser::Parse() {
  return Program();
}

AstNode Parser::Program() {
  AstNode head = AstNode(PROG_AST);
  GetNextTkn();

  head.AddChild(Statement(), 1);

  if (curr_tkn_.GetType() != EOF_TKN) {
    LogError("Expected end of file!");
  }

  return head;
}

AstNode Parser::Statement() {
  AstNode statement_ast = AstNode(EMPTY_AST);

  switch (curr_tkn_.GetType()) {
  case IF_TKN:
    statement_ast.SetType(IF_AST);
    GetNextTkn();

    statement_ast.AddChild(ParenExpr(), 1);
    statement_ast.AddChild(Statement(), 2);

    if (curr_tkn_.GetType() == ELSE_TKN) {
      statement_ast.SetType(ELSE_AST);
      GetNextTkn();
      statement_ast.AddChild(Statement(), 3);
    }
    break;
  case WHILE_TKN:
    statement_ast.SetType(WHILE_AST);
    GetNextTkn();
    statement_ast.AddChild(ParenExpr(), 1);
    statement_ast.AddChild(Statement(), 2);
    break;
  case SEMICOLON_TKN:
    GetNextTkn();
    break;
  case LEFT_BRACE_TKN:
    GetNextTkn();
    while (curr_tkn_.GetType() != RIGHT_BRACE_TKN) {
      AstNode brace_statement_ast = statement_ast;
      statement_ast = AstNode(SEQ_AST);

      statement_ast.AddChild(brace_statement_ast, 1);
      statement_ast.AddChild(Statement(), 2);
    }

    GetNextTkn();
    break;
  default:
    statement_ast.SetType(EXPR_AST);
    statement_ast.AddChild(Expr(), 1);
    if (curr_tkn_.GetType() == SEMICOLON_TKN) {
      GetNextTkn();
    } else {
      LogError("Expected semicolon");
    }

    break;
  }

  return statement_ast;
}

Token Parser::GetNextTkn() {
  curr_tkn_ = lexer_.Lex();
  return curr_tkn_;
}

void Parser::LogError(string error) {
  cout << error << endl;
}
