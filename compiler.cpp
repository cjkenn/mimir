#include <stdio.h>
#include <fstream>
#include <iostream>
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

  Lexer *lexer = new Lexer(filename);
  Parser parser = Parser(lexer);
  AstNode program = parser.Parse();

  Gen gen = Gen(&program, filename);
  gen.Generate(TYR_GEN);

  return 0;
}
