#include <iostream>
#include <sstream>
#include <unordered_map>
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
  case ID_TKN:
    return "Identifier";
  case NUM_TKN:
    return "Number";
  case IF_TKN:
    return "'If' keyword";
  case ELSE_TKN:
    return "'Else' keyword";
  case WHILE_TKN:
    return "'While' keyword";
  default:
    string type_str = "";
    return type_str + (char)type_;
  }
}
