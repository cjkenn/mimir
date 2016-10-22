#pragma once

#include <string>

enum TokenType {
  LEFT_PAREN_TKN = '(',
  RIGHT_PAREN_TKN = ')',
  LEFT_BRACE_TKN = '{',
  RIGHT_BRACE_TKN = '}',
  PLUS_TKN = '+',
  MINUS_TKN = '-',
  MULT_TKN = '*',
  DIV_TKN = '/',
  MOD_TKN = '%',
  ID_TKN,
  NUM_TKN,
  LT_TKN = '<',
  SEMICOLON_TKN = ';',
  EQUALS_TKN = '=',
  IF_TKN,
  ELSE_TKN,
  WHILE_TKN,
  EOF_TKN = EOF
};

class Token {
 public:
  Token();
  ~Token();
  Token(TokenType type);
  Token(TokenType type, int val);
  Token(TokenType type, std::string text);

  TokenType GetType() const {return type_;}
  int GetVal() const {return val_;}
  std::string GetText() const {return text_;}
  void Debug();

 private:
  void SetDebug();
  std::string GetPrettyType();

  TokenType type_;
  int val_;
  std::string text_;
  std::string debug_;
};
