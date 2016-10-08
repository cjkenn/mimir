#include "lexer.h"

Token lex(char curr_c)
{
  Token tkn = { EOF_TKN, 0, 0 };

  switch (curr_c) {
  case '(':
    tkn.type = LEFT_PAREN_TKN;
  case ')':
    tkn.type = RIGHT_PAREN_TKN;
  case '{':
    tkn.type = LEFT_BRACE_TKN;
  case '}':
    tkn.type = RIGHT_BRACE_TKN;
  case '+':
    tkn.type = PLUS_TKN;
  case '-':
    tkn.type = MINUS_TKN;
  case '*':
    tkn.type = MULT_TKN;
  case '/':
    tkn.type = DIV_TKN;
  case '%':
    tkn.type = MOD_TKN;

  }

  return tkn;
}
