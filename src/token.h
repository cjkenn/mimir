#pragma once

#include <string>

enum TokenType {
  LEFT_PAREN_TKN,
  RIGHT_PAREN_TKN,
  LEFT_BRACE_TKN,
  RIGHT_BRACE_TKN,
  PLUS_TKN,
  MINUS_TKN,
  STAR_TKN ,
  BACKSLASH_TKN ,
  PERCENT_TKN,
  EXCL_TKN,
  SEMICOLON_TKN,
  EQ_TKN,
  EQEQ_TKN,
  NEQ_TKN,
  LT_TKN,
  LTE_TKN,
  GT_TKN,
  GTE_TKN,
  ID_TKN,
  NUM_TKN,
  IF_TKN,
  ELSE_TKN,
  WHILE_TKN,
  EOF_TKN
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
  std::string GetPrettyType();
  void Debug();

 private:
  void SetDebug();

  TokenType type_;
  int val_;
  std::string text_;
  std::string debug_;
};
