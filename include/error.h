#pragma once

#include <string>
#include "token.h"

class Error {
 public:
  Error() {};
  Error(int line, int pos, std::string filename);
  ~Error() {};

  void SetMessage(std::string message) { message_ = message; }
  void Report();
  void SetMessageForExpect(TokenType expected, TokenType found);
  void SetMessageForUnknownToken(std::string tkn_val);
  void SetMessageForUnknownId(std::string id);

 private:
  std::string BuildLocErrorPrefix();
  std::string PrettifyTokenType(TokenType tkn_type);

  int line_;
  int pos_;
  std::string filename_;
  std::string message_;
};
