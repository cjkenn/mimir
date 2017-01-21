#include <string>
#include <iostream>
#include "token.h"
#include "parser.h"
#include "ast.h"

using namespace std;

Parser::Parser(std::shared_ptr<Lexer> lexer) {
  lexer_ = lexer;
}

AstNodePtr Parser::Parse() {
  return Program();
}

AstNodePtr Parser::Program() {
  auto head = std::make_shared<AstNode>(PROG_AST);
  GetNextTkn();

  // TODO: Can only parse one statement at a time
  head->AddChild(Statement());

  if (curr_tkn_.GetType() != EOF_TKN) {
    LogError("Expected end of file!");
  }

  return head;
}

AstNodePtr Parser::Statement() {
  auto statement_ast = std::make_shared<AstNode>(EMPTY_AST);

  switch (curr_tkn_.GetType()) {
  case IF_TKN:
    statement_ast->SetType(IF_AST);
    GetNextTkn();

    statement_ast->AddChild(ParenExpr());
    statement_ast->AddChild(Statement());

    if (curr_tkn_.GetType() == ELSE_TKN) {
      statement_ast->SetType(ELSE_AST);
      GetNextTkn();
      statement_ast->AddChild(Statement());
    }
    break;
  case WHILE_TKN:
    statement_ast->SetType(WHILE_AST);
    GetNextTkn();
    statement_ast->AddChild(ParenExpr());
    statement_ast->AddChild(Statement());
    break;
  case SEMICOLON_TKN:
    GetNextTkn();
    break;
  case LEFT_BRACE_TKN:
    GetNextTkn();
    while (curr_tkn_.GetType() != RIGHT_BRACE_TKN) {
      auto brace_statement_ast = statement_ast;
      statement_ast = std::make_shared<AstNode>(SEQ_AST);

      statement_ast->AddChild(brace_statement_ast);
      statement_ast->AddChild(Statement());
    }

    GetNextTkn();
    break;
  default:
    statement_ast->SetType(EXPR_AST);
    statement_ast->AddChild(Expr());

    Expect(SEMICOLON_TKN);
    break;
  }

  return statement_ast;
}

AstNodePtr Parser::ParenExpr() {
  Expect(LEFT_PAREN_TKN);
  AstNodePtr parenexpr_ast = Expr();
  Expect(RIGHT_PAREN_TKN);

  return parenexpr_ast;
}

AstNodePtr Parser::Expr() {
  if (curr_tkn_.GetType() != ID_TKN) {
    return Test();
  }

  AstNodePtr expr_ast = Test();

  if (expr_ast->GetType() == VAR_AST && curr_tkn_.GetType() == EQUALS_TKN) {
    AstNodePtr assign_ast = expr_ast;
    expr_ast = std::make_shared<AstNode>(SET_AST);
    GetNextTkn();

    expr_ast->AddChild(assign_ast);
    expr_ast->AddChild(Expr());
  }

  return expr_ast;
}

AstNodePtr Parser::Test() {
  AstNodePtr test_ast = Sum();

  if (curr_tkn_.GetType() == LT_TKN) {
    AstNodePtr less_ast = test_ast;
    test_ast = std::make_shared<AstNode>(LT_AST);
    GetNextTkn();

    test_ast->AddChild(less_ast);
    test_ast->AddChild(Sum());
  }

  return test_ast;
}

AstNodePtr Parser::Sum() {
  AstNodePtr sum_ast = Term();
  while (curr_tkn_.GetType() == PLUS_TKN || curr_tkn_.GetType() == MINUS_TKN) {
    AstNodePtr op_ast = sum_ast;

    AstType curr_type = ADD_AST;
    if (curr_tkn_.GetType() == MINUS_TKN) {
      curr_type = SUB_AST;
    }

    sum_ast = std::make_shared<AstNode>(curr_type);
    GetNextTkn();
    sum_ast->AddChild(op_ast);
    sum_ast->AddChild(Sum());
  }

  return sum_ast;
}

// Expect curr_tkn_ to be ident or number
AstNodePtr Parser::Term() {
  auto term_ast = std::make_shared<AstNode>(EMPTY_AST);
  TokenType curr_type = curr_tkn_.GetType();

  switch(curr_type) {
  case ID_TKN:
    term_ast->SetType(VAR_AST);
    term_ast->SetText(curr_tkn_.GetText());
    GetNextTkn();
    break;
  case NUM_TKN:
    term_ast->SetType(CST_AST);
    term_ast->SetVal(curr_tkn_.GetVal());
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
