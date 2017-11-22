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

const std::string Token::GetPrettyType() {
  switch (type_) {
  case TokenType::ID_TKN:
    return "Identifier";
  case TokenType::NUM_TKN:
    return "Number";
  case TokenType::IF_TKN:
    return "'If' keyword";
  case TokenType::ELSE_TKN:
    return "'Else' keyword";
  case TokenType::WHILE_TKN:
    return "'While' keyword";
  case TokenType::FUNC_TKN:
    return "'Function' keyword";
  case TokenType::LEFT_PAREN_TKN:
    return "(";
  case TokenType::RIGHT_PAREN_TKN:
    return ")";
  case TokenType::LEFT_BRACE_TKN:
    return "{";
  case TokenType::RIGHT_BRACE_TKN:
    return "}";
  case TokenType::PLUS_TKN:
    return "+";
  case TokenType::MINUS_TKN:
    return "-";
  case TokenType::STAR_TKN:
    return "*";
  case TokenType::BACKSLASH_TKN:
    return "/";
  case TokenType::PERCENT_TKN:
    return "%";
  case TokenType::SEMICOLON_TKN:
    return ";";
  case TokenType::COMMA_TKN:
    return ",";
  case TokenType::EQ_TKN:
    return "=";
  case TokenType::LT_TKN:
    return "<";
  case TokenType::LTE_TKN:
    return "<=";
  case TokenType::GT_TKN:
    return ">";
  case TokenType::GTE_TKN:
    return ">=";
  case TokenType::EQEQ_TKN:
    return "==";
  case TokenType::EOF_TKN:
    return "EOF";
  default:
    return "Unknown token type";
  }
}
