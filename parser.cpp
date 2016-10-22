#include <string>
#include <iostream>
#include "token.h"
#include "parser.h"
#include "ast.h"

using namespace std;

AstNode Parser::Parse() {
  return AstNode(EOF_AST);
}

Token Parser::GetTkn() {
  curr_tkn_ = lexer_.Lex();
  return curr_tkn_;
}

void Parser::LogError(string error) {
  cout << error << endl;
}
