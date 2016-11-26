#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <stdlib.h>
#include "gen.h"
#include "ast.h"

using namespace std;

Gen::Gen(AstNode *program, string filename) {
  filename_ = filename;
  program_ = program;
}

Gen::~Gen() {
  program_ = nullptr;
}

void Gen::Generate(GenTarget target) {
  switch (target) {
  case TYR_GEN:
    GenerateTyr();
    return;
  default:
    cout << "Invalid target specified!" << endl;
    exit(1);
  }
}

void Gen::GenerateTyr() {
  string outfile = filename_ + ".out";
  ofstream ofs(outfile.c_str());
  program_->Debug();
}
