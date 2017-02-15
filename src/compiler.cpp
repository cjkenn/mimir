#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "ast.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Please provide a filename!" << std::endl;
    std::cout << "Usage: mimir {filename}" << std::endl;
    exit(1);
  }

  std::string filename = argv[1];

  auto lexer = std::make_shared<Lexer>(filename);
  Parser parser = Parser(lexer);
  auto program = parser.Parse();

  return 0;
}
