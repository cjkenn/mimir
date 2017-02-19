#pragma once

#include <string>
#include "token.h"

class Error {
 public:
  Error(int line, int pos, std::string filename);
  ~Error() {};

  void SetMessage(std::string message);
  void Report();
  void SetMessageForExpect(TokenType expected, TokenType found);

 private:
  std::string BuildLocErrorPrefix();
  std::string PrettifyTokenType(TokenType tkn_type);

  int line_;
  int pos_;
  std::string filename_;
  std::string message_;
};
