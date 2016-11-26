#include <string>
#include <iostream>
#include "token.h"
#include "parser.h"
#include "ast.h"

using namespace std;

Parser::Parser(Lexer *lexer) {
  lexer_ = lexer;
}

Parser::~Parser() {
  delete lexer_;
}

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

    Expect(SEMICOLON_TKN);
    break;
  }

  return statement_ast;
}

AstNode Parser::ParenExpr() {
  Expect(LEFT_PAREN_TKN);
  AstNode parenexpr_ast = Expr();
  Expect(RIGHT_PAREN_TKN);

  return parenexpr_ast;
}

AstNode Parser::Expr() {
  if (curr_tkn_.GetType() != ID_TKN) {
    return Test();
  }

  AstNode expr_ast = Test();

  if (expr_ast.GetType() == VAR_AST && curr_tkn_.GetType() == EQUALS_TKN) {
    AstNode assign_ast = expr_ast;
    expr_ast = AstNode(SET_AST);
    GetNextTkn();

    expr_ast.AddChild(assign_ast, 1);
    expr_ast.AddChild(Expr(), 2);
  }
  return expr_ast;
}

AstNode Parser::Test() {
  AstNode test_ast = Sum();

  if (curr_tkn_.GetType() == LT_TKN) {
    AstNode less_ast = test_ast;
    test_ast = AstNode(LT_AST);
    GetNextTkn();

    test_ast.AddChild(less_ast, 1);
    test_ast.AddChild(Sum(), 2);
  }

  return test_ast;
}

AstNode Parser::Sum() {
  AstNode sum_ast = Term();
  while (curr_tkn_.GetType() == PLUS_TKN || curr_tkn_.GetType() == MINUS_TKN) {
    AstNode op_ast = sum_ast;

    AstType curr_type = ADD_AST;
    if (curr_tkn_.GetType() == MINUS_TKN) {
      curr_type = SUB_AST;
    }

    sum_ast = AstNode(curr_type);
    GetNextTkn();
    sum_ast.AddChild(op_ast, 1);
    sum_ast.AddChild(Sum(), 2);
  }

  return sum_ast;
}

// Expect curr_tkn_ to be ident or number
AstNode Parser::Term() {
  AstNode term_ast = AstNode(EMPTY_AST);
  TokenType curr_type = curr_tkn_.GetType();

  switch(curr_type) {
  case ID_TKN:
    term_ast.SetType(VAR_AST);
    term_ast.SetText(curr_tkn_.GetText());
    GetNextTkn();
    break;
  case NUM_TKN:
    term_ast.SetType(CST_AST);
    term_ast.SetVal(curr_tkn_.GetVal());
    GetNextTkn();
    break;
  default:
    term_ast = ParenExpr();
  }

  return term_ast;
}

void Parser::Expect(TokenType expected_type) {
  if (curr_tkn_.GetType() == expected_type) {
    GetNextTkn();
  } else {
    cout << "Expected " << expected_type << ", but found " << curr_tkn_.GetPrettyType() << endl;
    cout << "Current token: " << endl;
    curr_tkn_.Debug();
  }
}

void Parser::GetNextTkn() {
  curr_tkn_ = lexer_->Lex();
}

void Parser::LogError(string error) {
  cout << error << endl;
}
