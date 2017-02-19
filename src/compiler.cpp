#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "ast.h"
#include "symbol_table.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Please provide a filename!" << std::endl;
    std::cout << "Usage: mimir {filename}" << std::endl;
    exit(1);
  }

  std::string filename = argv[1];

  // TODO: Add in the error module here, use in lexer and parser
  auto error_manager = std::make_shared<Error>();
  auto lexer = std::make_shared<Lexer>(filename, error_manager);
  auto sym_tab = std::make_shared<SymbolTable>();

  Parser parser = Parser(lexer, sym_tab, error_manager);
  auto program = parser.Parse();

  return 0;
}
