#pragma once

#include <fstream>
#include "token.h"

class Lexer {
 public:
  Lexer();
  Lexer(std::string filename);
  ~Lexer();

  Token Lex();

 private:
  Token GetNumTkn();
  Token GetStrTkn();
  std::ifstream ifs_;
  char lastchar_;
};
