#include <iostream>
#include <string>
#include "token.h"
#include "error.h"

Error::Error(int line, int pos, std::string filename) {
  line_ = line;
  pos_ = pos;
  filename_ = filename;
}

void Error::SetMessageForExpect(TokenType expected, TokenType found) {
  std::string output;
  output += BuildLocErrorPrefix();
  output += "Expected '" +
    PrettifyTokenType(expected) + "'"
    ", but found '" +
    PrettifyTokenType(found) + "'";

  message_ = output;
}

void Error::SetMessageForUnknownToken(std::string tkn_val) {
  std::string output = BuildLocErrorPrefix();
  output += "Unknown token '" + tkn_val + "' found.";

  message_ = output;
}

void Error::Report() {
  std::cout << message_ << std::endl;
}

std::string Error::BuildLocErrorPrefix() {
  std::string output;
  output += filename_ + " -> ";
  output += "At line " + std::to_string(line_) + ":" + std::to_string(pos_) + " ";

  return output;
}

std::string Error::PrettifyTokenType(TokenType tkn_type) {
  switch (tkn_type) {
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
  case TokenType::EOF_TKN:
    return "EOF";
  default:
    return "Unknown token type";
  }
}
