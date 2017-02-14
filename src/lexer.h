#pragma once

#include <fstream>
#include "token.h"

class Lexer {
 public:
  Lexer();
  Lexer(std::string filename);
  ~Lexer();

  // Read the file stream and return the next Token from the file.
  Token Lex();

 private:
  Token GetNumTkn();
  Token GetStrTkn();

  // Move the char pointer ahead one, and if we are at the end, ensure
  // that we will get and EOF token.
  void Advance();

  std::ifstream ifs_;
  char lastchar_;
};
