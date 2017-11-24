#pragma once

#include <string>

namespace mimir {
enum TokenType {
  LEFT_PAREN_TKN,
  RIGHT_PAREN_TKN,
  LEFT_BRACE_TKN,
  RIGHT_BRACE_TKN,
  PLUS_TKN,
  MINUS_TKN,
  STAR_TKN,
  BACKSLASH_TKN,
  PERCENT_TKN,
  EXCL_TKN,
  SEMICOLON_TKN,
  COMMA_TKN,
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
  FUNC_TKN,
  UNKNOWN_TKN,
  EOF_TKN
};

class Token {
 public:
  Token() {};
  ~Token() {};
  Token(const TokenType type);
  Token(const TokenType type, const int val);
  Token(const TokenType type, const std::string text);

  TokenType GetType() const { return type_; }
  int GetVal() const { return val_; }
  int GetLinePos() const { return line_pos_; }
  int GetCharPos() const { return char_pos_; }
  std::string GetText() const { return text_; }

  void SetType(const TokenType type) { type_ = type; }
  void SetText(const std::string text) { text_ = text; }
  void SetLinePos(const int line) { line_pos_ = line; }
  void SetCharPos(const int ch) { char_pos_ = ch; }

 private:
  TokenType type_;
  int val_;
  std::string text_;
  int line_pos_;
  int char_pos_;
};
}
