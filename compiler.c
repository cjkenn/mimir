#include <stdio.h>
#include "lexer.h"

int main()
{
  Token t = lex();
  printf("Token type: %d, val: %s\n", t.type, t.value);

  return 0;
}
