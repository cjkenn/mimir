#include <stdio.h>
#include <fstream>
#include <iostream>
#include "lexer.h"
#include "parser.h"

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
  parser.Parse();

  return 0;
}
