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
  EOF_TKN
};

class Token
{
public:
  Token() {};
 Token(TokenType type) : m_type(type) {};
 Token(TokenType type, int val) : m_type(type), m_val(val) {};
 Token(TokenType type, std::string text) : m_type(type), m_text(text) {};

  TokenType get_type() const {return m_type;}
  int get_val() const {return m_val;}
  std::string get_text() const {return m_text;}

private:
  TokenType m_type;
  int m_val;
  std::string m_text;
};
