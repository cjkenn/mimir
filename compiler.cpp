#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main() {
  Lexer *lexer = new Lexer();
  Parser parser = Parser(lexer);
  parser.Parse();

  return 0;
}
