#include <stdio.h>
#include <fstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
  std::string filename = argv[1];

  Lexer *lexer = new Lexer(filename);
  Parser parser = Parser(lexer);
  parser.Parse();

  return 0;
}
