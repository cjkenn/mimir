#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "gen.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Please provide a filename!" << endl;
    cout << "Usage: mimir {filename}" << endl;
    exit(1);
  }

  std::string filename = argv[1];

  auto lexer = std::make_shared<Lexer>(filename);
  Parser parser = Parser(lexer);
  auto program = parser.Parse();

  Gen gen = Gen(program, filename);
  gen.Generate(TYR_GEN);

  return 0;
}
