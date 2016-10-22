#pragma once

#include <string>

enum TokenType
{
  LEFT_PAREN_TKN,
  RIGHT_PAREN_TKN,
  LEFT_BRACE_TKN,
  RIGHT_BRACE_TKN,
  PLUS_TKN,
  MINUS_TKN,
  MULT_TKN,
  DIV_TKN,
  MOD_TKN,
  ID_TKN,
  NUM_TKN,
  STR_TKN,
  LT_TKN,
  SEMICOLON_TKN,
  EQUALS_TKN,
  EOF_TKN
};

class Token
{
 public:
  Token() {};
  Token(TokenType type) : type_(type) {};
  Token(TokenType type, int val) : type_(type), val_(val) {};
  Token(TokenType type, std::string text) : type_(type), text_(text) {};

  TokenType get_type() const {return type_;}
  int get_val() const {return val_;}
  std::string get_text() const {return text_;}

 private:
  TokenType type_;
  int val_;
  std::string text_;
};
