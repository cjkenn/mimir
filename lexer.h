#pragma once

#include "token.h"

class Lexer
{
public:
  Token lex();

private:
  Token get_num_tkn(int curr_c);
  Token get_str_tkn(int curr_c);
};
