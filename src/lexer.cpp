#include <string>
#include <cstdio>
#include <stdlib.h>
#include <memory>
#include <fstream>
#include <iostream>
#include "token.h"
#include "lexer.h"
#include "error.h"

using namespace mimir;

Lexer::Lexer(const std::string filename) {
  filename_ = filename;
  curr_line_pos_ = 1;
  curr_char_pos_ = 1;
  ifs_.open(filename.c_str());
  ifs_.get(lastchar_);
}

Lexer::~Lexer() {
  ifs_.close();
}

Token Lexer::Lex() {
  Token tkn;

  if (lastchar_ == -1) {
    tkn = BuildToken(TokenType::EOF_TKN);
    return tkn;
  }

  // Skip whitespace
  while (isspace(lastchar_)) {
    curr_char_pos_++;
    if (lastchar_ == '\n') {
      curr_line_pos_++;
      curr_char_pos_ = 1;
    }

    if (!ifs_.get(lastchar_)) {
      tkn = BuildToken(TokenType::EOF_TKN);
      return tkn;
    }
  }

  // Lex a number
  if (isdigit(lastchar_)) {
    return GetNumTkn();
  }

  // Lex a string
    if (isalpha(lastchar_)) {
    return GetStrTkn();
  }

  // If we don't have a number or a string, we can lex a single
  // character symbol.
  switch (lastchar_) {
  case '(':
    tkn = BuildTokenAndAdvance(TokenType::LEFT_PAREN_TKN);
    return tkn;
  case ')':
    tkn = BuildTokenAndAdvance(TokenType::RIGHT_PAREN_TKN);
    return tkn;
  case '{':
    tkn = BuildTokenAndAdvance(TokenType::LEFT_BRACE_TKN);
    return tkn;
  case '}':
    tkn = BuildTokenAndAdvance(TokenType::RIGHT_BRACE_TKN);
    return tkn;
  case '+':
    tkn = BuildTokenAndAdvance(TokenType::PLUS_TKN);
    return tkn;
  case '-':
    tkn = BuildTokenAndAdvance(TokenType::MINUS_TKN);
    return tkn;
  case '*':
    tkn = BuildTokenAndAdvance(TokenType::STAR_TKN);
    return tkn;
  case '/':
    tkn = BuildTokenAndAdvance(TokenType::BACKSLASH_TKN);
    return tkn;
  case '%':
    tkn = BuildTokenAndAdvance(TokenType::PERCENT_TKN);
    return tkn;
  case '<':
    {
      const char next = Peek();
      const int start_pos = curr_char_pos_;
      if (next == '=') {
	tkn = Token(TokenType::LTE_TKN);
	Advance();
      } else {
	tkn = Token(TokenType::LT_TKN);
      }
      Advance();

      tkn.SetLinePos(curr_line_pos_);
      tkn.SetCharPos(start_pos);
      return tkn;
    }
  case '>':
    {
      const char next = Peek();
      const int start_pos = curr_char_pos_;
      if (next == '=') {
	tkn = Token(TokenType::GTE_TKN);
	Advance();
      } else {
	tkn = Token(TokenType::GT_TKN);
      }
      Advance();

      tkn.SetLinePos(curr_line_pos_);
      tkn.SetCharPos(start_pos);
      return tkn;
    }
  case '=':
    {
      const char next = Peek();
      const int start_pos = curr_char_pos_;
      if (next == '=') {
	tkn = Token(TokenType::EQEQ_TKN);
	Advance();
      } else {
	tkn = Token(TokenType::EQ_TKN);
      }
      Advance();

      tkn.SetLinePos(curr_line_pos_);
      tkn.SetCharPos(start_pos);
      return tkn;
    }
  case '!':
    {
      const char next = Peek();
      const int start_pos = curr_char_pos_;
      if (next == '=') {
	tkn = Token(TokenType::NEQ_TKN);
	Advance();
      } else {
	tkn = Token(TokenType::EXCL_TKN);
      }
      Advance();

      tkn.SetLinePos(curr_line_pos_);
      tkn.SetCharPos(start_pos);
      return tkn;
    }
  case ';':
    tkn = BuildTokenAndAdvance(TokenType::SEMICOLON_TKN);
    return tkn;
  default:
    {
      // Set the token type as unknown is we don't recognize it here. This
      // should cause an expect error to be reported by the parser later on.
      tkn = BuildTokenAndAdvance(TokenType::UNKNOWN_TKN);
      break;
    }
  }

  return tkn;
}

Token Lexer::BuildTokenAndAdvance(const TokenType curr_type) {
  Token tkn(curr_type);
  tkn.SetLinePos(curr_line_pos_);
  tkn.SetCharPos(curr_char_pos_);
  tkn.SetText(std::string(1, lastchar_));

  Advance();

  return tkn;
}

Token Lexer::BuildToken(const TokenType curr_type) {
  Token tkn(curr_type);
  tkn.SetLinePos(curr_line_pos_);
  tkn.SetCharPos(curr_char_pos_);
  tkn.SetText(std::string(1, lastchar_));

  return tkn;
}

Token Lexer::GetNumTkn() {
  std::string num_str;
  const int start_char_pos = curr_char_pos_;
  num_str += lastchar_;

  Advance();

  while (isdigit(lastchar_)) {
    num_str += lastchar_;
    Advance();
  }
  const int val = strtod(num_str.c_str(), 0);

  Token tkn = Token(TokenType::NUM_TKN, val);
  tkn.SetLinePos(curr_line_pos_);
  tkn.SetCharPos(start_char_pos);

  return tkn;
}

Token Lexer::GetStrTkn() {
  std::string ident;
  const int start_char_pos = curr_char_pos_;
  ident += lastchar_;

  Advance();

  while (isalnum(lastchar_)) {
    ident += lastchar_;
    Advance();
  }

  Token tkn = Token(TokenType::ID_TKN, ident);

  if (ident == "while") {
    tkn.SetType(TokenType::WHILE_TKN);
  }

  if (ident == "if") {
    tkn.SetType(TokenType::IF_TKN);
  }

  if (ident == "else") {
    tkn.SetType(TokenType::ELSE_TKN);
  }

  tkn.SetLinePos(curr_line_pos_);
  tkn.SetCharPos(start_char_pos);

  return tkn;
}

void Lexer::Advance() {
  if (!ifs_.get(lastchar_)) {
    lastchar_ = -1;
  }
  curr_char_pos_++;
}

char Lexer::Peek() {
  return ifs_.peek();
}
