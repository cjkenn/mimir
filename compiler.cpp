#include <stdio.h>
#include "lexer.h"

int main()
{
  Lexer lexer = Lexer();
  Token tkn = lexer.lex();
  printf("Token type: %d, val: %d\n", tkn.get_type(), tkn.get_val());

  return 0;
}
