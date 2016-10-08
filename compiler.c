#include <stdio.h>
#include "lexer.h"

int main() {
  Token t = lex('{');
  printf("Token type: %d, line: %d, pos: %d\n", t.type, t.line, t.pos);

  return 0;
}
