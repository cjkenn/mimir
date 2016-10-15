#include "token.h"

using namespace std;

Token::Token() {}

Token::Token(TokenType type)
{
  m_type = type;
}

Token::Token(TokenType type, int val)
{
  m_type = type;
  m_val = val;
}

Token::Token(TokenType type, string text)
{
  m_type = type;
  m_text = text;
}
