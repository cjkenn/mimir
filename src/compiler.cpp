#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "ast.h"
#include "symbol_table.h"
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
  auto sym_tab = std::make_shared<SymbolTable>();

  Parser parser = Parser(lexer, sym_tab);
  auto program = parser.Parse();

  return 0;
}
