#include "lexer.h"
#include "token.h"
#include <string>
#include <cstdio>
#include <stdlib.h>

using namespace std;

Token Lexer::Lex() {
  Token tkn;
  int lastchar = ' ';

  // Skip whitespace
  while (isspace(lastchar)) {
    lastchar = getchar();
  }

  // Lex a single character symbol
  switch (lastchar) {
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
  if (isdigit(lastchar)) {
    return GetNumTkn(lastchar);
  }

  // Lex a string
  if (isalpha(lastchar)) {
    return GetStrTkn(lastchar);
  }

  return tkn;
}

Token Lexer::GetNumTkn(int curr_c) {
  string num_str;

  do {
    num_str += curr_c;
    curr_c = getchar();
  } while (isdigit(curr_c));

  int val = strtod(num_str.c_str(), 0);

  return Token(NUM_TKN, val);
}

Token Lexer::GetStrTkn(int curr_c) {
  string ident = to_string(curr_c);

  while (isalnum((curr_c = getchar()))) {
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

  return Token(STR_TKN, ident);
}
