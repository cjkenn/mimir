#include <string>
#include <iostream>
#include "token.h"
#include "parser.h"
#include "ast/expr.h"

using namespace std;

ExprAST Parser::parse() {
  return ExprAST();
}

Token Parser::get_tkn() {
  return Token();
}

void Parser::log_error(string error) {
  cout << error << endl;
}
