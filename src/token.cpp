#include "token.h"

using namespace mimir;

Token::Token(const TokenType type) {
  type_ = type;
}

Token::Token(const TokenType type, const int val) {
  type_ = type;
  val_ = val;
}

Token::Token(const TokenType type, const std::string text) {
  type_ = type;
  text_ = text;
}
