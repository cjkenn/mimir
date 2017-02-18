#include <iostream>
#include <sstream>
#include "token.h"

Token::Token(TokenType type) {
  type_ = type;
  SetDebug();
}

Token::Token(TokenType type, int val) {
  type_ = type;
  val_ = val;
  SetDebug();
}

Token::Token(TokenType type, std::string text) {
  type_ = type;
  text_ = text;
  SetDebug();
}

void Token::SetDebug() {
  std::stringstream debug_stream;
  debug_stream << "{\n TokenType: " << GetPrettyType() <<
    "\n Value: " << val_ <<
    "\n Text: " << text_ <<
    "\n}";

  debug_ = debug_stream.str();
}

void Token::Debug() {
  std::cout << debug_ << std::endl;
}

std::string Token::GetPrettyType() {
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
  default:
    return "Unknown token type";
  }
}
