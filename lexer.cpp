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
  ifs_.open(filename.c_str(), std::ifstream::in);
}

Lexer::~Lexer() {
  ifs_.close();
}

Token Lexer::Lex() {
  Token tkn;

  // Skip whitespace
  while (isspace(lastchar_)) {
    lastchar_ = ifs_.get();
  }

  // Lex a single character symbol
  switch (lastchar_) {
  case '(':
    tkn = Token(LEFT_PAREN_TKN);
    break;
  case ')':
    tkn = Token(RIGHT_PAREN_TKN);
    break;
  case '{':
    tkn = Token(LEFT_BRACE_TKN);
    break;
  case '}':
    tkn = Token(RIGHT_BRACE_TKN);
    break;
  case '+':
    tkn = Token(PLUS_TKN);
    break;
  case '-':
    tkn = Token(MINUS_TKN);
    break;
  case '*':
    tkn = Token(MULT_TKN);
    break;
  case '/':
    tkn = Token(DIV_TKN);
    break;
  case '%':
    tkn = Token(MOD_TKN);
    break;
  case '<':
    tkn = Token(LT_TKN);
    break;
  case ';':
    tkn = Token(SEMICOLON_TKN);
    break;
  case '=':
    tkn = Token(EQUALS_TKN);
    break;
  case EOF:
    tkn = Token(EOF_TKN);
    break;
  }

  // Lex a number
  if (isdigit(lastchar_)) {
    return GetNumTkn(lastchar_);
  }

  // Lex a string
  if (isalpha(lastchar_)) {
    return GetStrTkn(lastchar_);
  }

  return tkn;
}

Token Lexer::GetNumTkn(int curr_c) {
  string num_str;

  do {
    num_str += curr_c;
    curr_c = ifs_.get();
  } while (isdigit(curr_c));

  int val = strtod(num_str.c_str(), 0);

  return Token(NUM_TKN, val);
}

Token Lexer::GetStrTkn(int curr_c) {
  string ident = "";
  ident += curr_c;

  while (isalnum((curr_c = ifs_.get()))) {
    ident += curr_c;
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
