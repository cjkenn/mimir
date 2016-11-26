#include <iostream>
#include <fstream>
#include <string>
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
  // TODO: Better error handling here?
  if (program_->GetType() != PROG_AST) {
    cout << "Incorrect program type provided" << endl;
    exit(1);
  }

  string outfile = filename_ + ".out";
  ofstream ofs(outfile.c_str());
  AstNode *instr = program_->GetFirstChild();

  switch(instr->GetType()) {
  case VAR_AST:
    EmitVar(instr);
    break;
  case EXPR_AST:
    EmitExpr(instr);
    break;
  }

}

void Gen::EmitVar(AstNode *node) {

}

void Gen::EmitExpr(AstNode *node) {
}
