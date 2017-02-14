#include <iostream>
#include <sstream>
#include "token.h"

using namespace std;

Token::Token() {}

Token::~Token() {}

Token::Token(TokenType type) {
  type_ = type;
  SetDebug();
}

Token::Token(TokenType type, int val) {
  type_ = type;
  val_ = val;
  SetDebug();
}

Token::Token(TokenType type, string text) {
  type_ = type;
  text_ = text;
  SetDebug();
}

void Token::SetDebug() {
  stringstream debug_stream;
  debug_stream << "{\n TokenType: " << GetPrettyType() <<
    "\n Value: " << val_ <<
    "\n Text: " << text_ <<
    "\n}";

  debug_ = debug_stream.str();
}

void Token::Debug() {
  cout << debug_ << endl;
}

string Token::GetPrettyType() {
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
  default:
    string type_str = "";
    return type_str + (char)type_;
  }
}
