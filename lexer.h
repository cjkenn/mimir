#pragma once

#include "token.h"

class Lexer {
 public:
  Token Lex();

 private:
  Token GetNumTkn(int curr_c);
  Token GetStrTkn(int curr_c);
};
