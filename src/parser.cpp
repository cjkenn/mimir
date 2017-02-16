#include <string>
#include <iostream>
#include <stdlib.h>
#include "token.h"
#include "parser.h"
#include "ast.h"

Parser::Parser(std::shared_ptr<Lexer> lexer) {
  lexer_ = lexer;
}

AstNodePtr Parser::Parse() {
  return Program();
}

AstNodePtr Parser::Program() {
  auto head = std::make_shared<AstNode>(AstType::PROG_AST);
  GetNextTkn();

  // TODO: Can only parse one statement at a time
  while (curr_tkn_.GetType() != TokenType::EOF_TKN) {
    head->AddChild(Statement());
  }

  return head;
}

AstNodePtr Parser::Statement() {
  auto statement_ast = std::make_shared<AstNode>(AstType::EMPTY_AST);

  switch (curr_tkn_.GetType()) {
  case TokenType::IF_TKN:
    statement_ast->SetType(AstType::IF_AST);
    GetNextTkn();
    statement_ast->AddChild(ParenExpr());
    statement_ast->AddChild(Statement());

    if (curr_tkn_.GetType() == TokenType::ELSE_TKN) {
      statement_ast->SetType(AstType::ELSE_AST);
      GetNextTkn();
      statement_ast->AddChild(Statement());
    }

    break;
  case TokenType::WHILE_TKN:
    statement_ast->SetType(AstType::WHILE_AST);
    GetNextTkn();
    statement_ast->AddChild(ParenExpr());
    statement_ast->AddChild(Statement());

    break;
  case TokenType::SEMICOLON_TKN:
    GetNextTkn();
    break;
  case TokenType::LEFT_BRACE_TKN:
    GetNextTkn();
    while (curr_tkn_.GetType() != TokenType::RIGHT_BRACE_TKN) {
      auto brace_statement_ast = statement_ast;
      statement_ast = std::make_shared<AstNode>(AstType::SEQ_AST);

      if (brace_statement_ast->GetType() != AstType::EMPTY_AST) {
	statement_ast->AddChild(brace_statement_ast);
      }

      statement_ast->AddChild(Statement());
    }

    GetNextTkn();
    break;
  default:
    statement_ast->SetType(AstType::EXPR_AST);
    statement_ast->AddChild(Expr());

    Expect(TokenType::SEMICOLON_TKN);
    break;
  }

  return statement_ast;
}

AstNodePtr Parser::ParenExpr() {
  Expect(TokenType::LEFT_PAREN_TKN);
  AstNodePtr parenexpr_ast = Expr();
  Expect(TokenType::RIGHT_PAREN_TKN);

  return parenexpr_ast;
}

AstNodePtr Parser::Expr() {
  if (curr_tkn_.GetType() != TokenType::ID_TKN) {
    return Test();
  }

  AstNodePtr expr_ast = Test();

  if (expr_ast->GetType() == AstType::VAR_AST && curr_tkn_.GetType() == TokenType::EQ_TKN) {
    AstNodePtr assign_ast = expr_ast;
    expr_ast = std::make_shared<AstNode>(AstType::SET_AST);
    GetNextTkn();

    expr_ast->AddChild(assign_ast);
    expr_ast->AddChild(Expr());
  }

  return expr_ast;
}

AstNodePtr Parser::Test() {
  AstNodePtr test_ast = BinOp();

  if (CurrTknIsTestTkn()) {
    AstNodePtr less_ast = test_ast;
    AstType curr_ast_type = GetTestAstFromTkn();

    test_ast = std::make_shared<AstNode>(curr_ast_type);
    GetNextTkn();

    test_ast->AddChild(less_ast);
    test_ast->AddChild(BinOp());
  }

  return test_ast;
}

AstNodePtr Parser::BinOp() {
  AstNodePtr sum_ast = Term();

  while (CurrTknIsBinOpTkn()) {
    AstNodePtr op_ast = sum_ast;
    AstType curr_ast_type = GetBinOpAstFromTkn();

    sum_ast = std::make_shared<AstNode>(curr_ast_type);
    GetNextTkn();
    sum_ast->AddChild(op_ast);
    sum_ast->AddChild(BinOp());
  }

  return sum_ast;
}

// Expect curr_tkn_ to be ident or number
AstNodePtr Parser::Term() {
  auto term_ast = std::make_shared<AstNode>(AstType::EMPTY_AST);
  TokenType curr_type = curr_tkn_.GetType();

  switch(curr_type) {
  case TokenType::ID_TKN:
    term_ast->SetType(AstType::VAR_AST);
    term_ast->SetText(curr_tkn_.GetText());
    GetNextTkn();
    break;
  case TokenType::NUM_TKN:
    term_ast->SetType(AstType::CST_AST);
    term_ast->SetVal(curr_tkn_.GetVal());
    GetNextTkn();
    break;
  default:
    term_ast = ParenExpr();
  }

  return term_ast;
}

// TODO: Error handling module
void Parser::Expect(TokenType expected_type) {
  if (curr_tkn_.GetType() == expected_type) {
    GetNextTkn();
  } else {
    std::cout << "Expected " <<
      PrettifyTokenType(expected_type) <<
      ", but found " <<
      curr_tkn_.GetPrettyType() << std::endl;

    std::cout << "Current token: " << std::endl;
    curr_tkn_.Debug();
    std::exit(1);
  }
}

void Parser::GetNextTkn() {
  curr_tkn_ = lexer_->Lex();
}

bool Parser::CurrTknIsBinOpTkn() {
  TokenType curr_type = curr_tkn_.GetType();
  return (curr_type == TokenType::PLUS_TKN ||
	  curr_type == TokenType::MINUS_TKN ||
	  curr_type == TokenType::STAR_TKN ||
	  curr_type == TokenType::BACKSLASH_TKN ||
	  curr_type == TokenType::PERCENT_TKN);
}

AstType Parser::GetBinOpAstFromTkn() {
  TokenType curr_type = curr_tkn_.GetType();

  switch (curr_type) {
  case TokenType::PLUS_TKN:
    return AstType::ADD_AST;
  case TokenType::MINUS_TKN:
    return AstType::SUB_AST;
  case TokenType::STAR_TKN:
    return AstType::MUL_AST;
  case TokenType::BACKSLASH_TKN:
    return AstType::DIV_AST;
  case TokenType::PERCENT_TKN:
    return AstType::MOD_AST;
  default:
    return AstType::EMPTY_AST;
  }
}

bool Parser::CurrTknIsTestTkn() {
  TokenType curr_type = curr_tkn_.GetType();
  return (curr_type == TokenType::GT_TKN ||
	  curr_type == TokenType::LT_TKN ||
	  curr_type == TokenType::LTE_TKN ||
	  curr_type == TokenType::GTE_TKN ||
	  curr_type == TokenType::EQEQ_TKN ||
	  curr_type == TokenType::NEQ_TKN);
}

AstType Parser::GetTestAstFromTkn() {
  TokenType curr_type = curr_tkn_.GetType();

  switch (curr_type) {
  case TokenType::LT_TKN:
    return AstType::LT_AST;
  case TokenType::GT_TKN:
    return AstType::GT_AST;
  case TokenType::LTE_TKN:
    return AstType::LTE_AST;
  case TokenType::GTE_TKN:
    return AstType::GTE_AST;
  case TokenType::EQEQ_TKN:
    return AstType::EQEQ_AST;
  case TokenType::NEQ_TKN:
    return AstType::NEQ_AST;
  default:
    return AstType::EMPTY_AST;
  }
}

void Parser::LogError(std::string error) {
  std::cout << error << std::endl;
}

// TODO: Error handling module
std::string Parser::PrettifyTokenType(TokenType type) {
  switch (type) {
  case TokenType::ID_TKN:
    return "Identifier";
  case TokenType::NUM_TKN:
    return "Number";
  case TokenType::IF_TKN:
    return "'If' keyword";
  case TokenType::ELSE_TKN:
    return "'Else' keyword";
  case TokenType::WHILE_TKN:
    return "'While' keyword";
  default:
    std::string type_str = "";
    return type_str + (char)type;
  }
}
