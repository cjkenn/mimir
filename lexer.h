#pragma once

#include "token.h"
#include <fstream>

class Lexer {
 public:
  Lexer();
  Lexer(std::string filename);
  ~Lexer();

  Token Lex();

 private:
  Token GetNumTkn(int curr_c);
  Token GetStrTkn(int curr_c);
  std::ifstream ifs_;
  char lastchar_;
};
