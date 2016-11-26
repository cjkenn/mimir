#include <string>
#include <cstdio>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "token.h"
#include "lexer.h"

using namespace std;

Lexer::Lexer() {
  lastchar_ = ' ';
}

Lexer::Lexer(string filename) {
  lastchar_ = ' ';
  ifs_.open(filename.c_str());
}

Lexer::~Lexer() {
  ifs_.close();
}

Token Lexer::Lex() {
  Token tkn;

  // Skip whitespace
  while (isspace(lastchar_)) {
    if (!ifs_.get(lastchar_)) {
      break;
    }
  }

  // Lex a single character symbol
  switch (lastchar_) {
  case '(':
    tkn = Token(LEFT_PAREN_TKN);
    ifs_.get(lastchar_);
    break;
  case ')':
    tkn = Token(RIGHT_PAREN_TKN);
    ifs_.get(lastchar_);
    break;
  case '{':
    tkn = Token(LEFT_BRACE_TKN);
    ifs_.get(lastchar_);
    break;
  case '}':
    tkn = Token(RIGHT_BRACE_TKN);
    ifs_.get(lastchar_);
    break;
  case '+':
    tkn = Token(PLUS_TKN);
    ifs_.get(lastchar_);
    break;
  case '-':
    tkn = Token(MINUS_TKN);
    ifs_.get(lastchar_);
    break;
  case '*':
    tkn = Token(MULT_TKN);
    ifs_.get(lastchar_);
    break;
  case '/':
    tkn = Token(DIV_TKN);
    ifs_.get(lastchar_);
    break;
  case '%':
    tkn = Token(MOD_TKN);
    ifs_.get(lastchar_);
    break;
  case '<':
    tkn = Token(LT_TKN);
    ifs_.get(lastchar_);
    break;
  case ';':
    tkn = Token(SEMICOLON_TKN);
    ifs_.get(lastchar_);
    break;
  case '=':
    tkn = Token(EQUALS_TKN);
    ifs_.get(lastchar_);
    break;
  default:
    tkn = Token(EOF_TKN);
    break;
  }

  // Lex a number
  if (isdigit(lastchar_)) {
    return GetNumTkn();
  }

  // Lex a string
  if (isalpha(lastchar_)) {
    return GetStrTkn();
  }

  return tkn;
}

Token Lexer::GetNumTkn() {
  string num_str = "";
  num_str += lastchar_;
  ifs_.get(lastchar_);

  while (isdigit(lastchar_)) {
    num_str += lastchar_;
    ifs_.get(lastchar_);
  }
  int val = strtod(num_str.c_str(), 0);

  return Token(NUM_TKN, val);
}

Token Lexer::GetStrTkn() {
  string ident = "";
  ident += lastchar_;

  ifs_.get(lastchar_);

  while (isalnum(lastchar_)) {
    ident += lastchar_;
    ifs_.get(lastchar_);
  }

  if (ident == "while") {
    return Token(WHILE_TKN, ident);
  }

  if (ident == "if") {
    return Token(IF_TKN, ident);
  }

  if (ident == "else") {
    return Token(ELSE_TKN, ident);
  }

  return Token(ID_TKN, ident);
}
