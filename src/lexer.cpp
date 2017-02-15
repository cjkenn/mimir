#include <string>
#include <cstdio>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "token.h"
#include "lexer.h"

Lexer::Lexer() {
  lastchar_ = ' ';
}

Lexer::Lexer(std::string filename) {
  lastchar_ = ' ';
  ifs_.open(filename.c_str());
}

Lexer::~Lexer() {
  ifs_.close();
}

Token Lexer::Lex() {
  Token tkn;

  if (lastchar_ == -1) {
    tkn = Token(TokenType::EOF_TKN);
    return tkn;
  }

  // Skip whitespace
  while (isspace(lastchar_)) {
    if (!ifs_.get(lastchar_)) {
      tkn = Token(TokenType::EOF_TKN);
      return tkn;
    }
  }

  // Lex a single character symbol
  switch (lastchar_) {
  case '(':
    tkn = Token(TokenType::LEFT_PAREN_TKN);
    Advance();
    return tkn;
  case ')':
    tkn = Token(TokenType::RIGHT_PAREN_TKN);
    Advance();
    return tkn;
  case '{':
    tkn = Token(TokenType::LEFT_BRACE_TKN);
    Advance();
    return tkn;
  case '}':
    tkn = Token(TokenType::RIGHT_BRACE_TKN);
    Advance();
    return tkn;
  case '+':
    tkn = Token(TokenType::PLUS_TKN);
    Advance();
    return tkn;
  case '-':
    tkn = Token(TokenType::MINUS_TKN);
    Advance();
    return tkn;
  case '*':
    tkn = Token(TokenType::STAR_TKN);
    Advance();
    return tkn;
  case '/':
    tkn = Token(TokenType::BACKSLASH_TKN);
    Advance();
    return tkn;
  case '%':
    tkn = Token(TokenType::PERCENT_TKN);
    Advance();
    return tkn;
  case '<':
    {
      char next = Peek();
      if (next == '=') {
	tkn = Token(TokenType::LTE_TKN);
	Advance();
      } else {
	tkn = Token(TokenType::LT_TKN);
      }
      Advance();
      return tkn;
    }
  case '>':
    {
      char next = Peek();
      if (next == '=') {
	tkn = Token(TokenType::GTE_TKN);
	Advance();
      } else {
	tkn = Token(TokenType::GT_TKN);
      }
      Advance();
      return tkn;
    }
  case ';':
    tkn = Token(TokenType::SEMICOLON_TKN);
    Advance();
    return tkn;
  case '=':
    tkn = Token(TokenType::EQUALS_TKN);
    Advance();
    return tkn;
  default:
    tkn = Token(TokenType::EOF_TKN);
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
  std::string num_str = "";
  num_str += lastchar_;
  ifs_.get(lastchar_);

  while (isdigit(lastchar_)) {
    num_str += lastchar_;
    ifs_.get(lastchar_);
  }
  int val = strtod(num_str.c_str(), 0);

  return Token(TokenType::NUM_TKN, val);
}

Token Lexer::GetStrTkn() {
  std::string ident = "";
  ident += lastchar_;

  ifs_.get(lastchar_);

  while (isalnum(lastchar_)) {
    ident += lastchar_;
    ifs_.get(lastchar_);
  }

  if (ident == "while") {
    return Token(TokenType::WHILE_TKN, ident);
  }

  if (ident == "if") {
    return Token(TokenType::IF_TKN, ident);
  }

  if (ident == "else") {
    return Token(TokenType::ELSE_TKN, ident);
  }

  return Token(TokenType::ID_TKN, ident);
}

void Lexer::Advance() {
  if (!ifs_.get(lastchar_)) {
    lastchar_ = -1;
  }
}

char Lexer::Peek() {
  return ifs_.peek();
}
