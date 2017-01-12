#include <iostream>
#include <fstream>
#include <string>
#include "gen.h"
#include "ast.h"

using namespace std;

Gen::Gen(AstNodePtr program, string filename) {
  filename_ = filename + ".out";
  program_ = program;
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
  AstNodePtr instr = program_->GetFirstChild();

  switch(instr->GetType()) {
  case VAR_AST:
    EmitVar(instr);
    break;
  case EXPR_AST:
    EmitExpr(instr);
    break;
  }

}

void Gen::EmitVar(AstNodePtr node) {

}

// Expect:
// Expression AST
//   -> Assignment AST
//        -> Var AST
//        -> Constant AST
void Gen::EmitExpr(AstNodePtr node) {
  AstNodePtr assign = node->GetFirstChild();
  string name = assign->GetFirstChild()->GetText();
  int val = assign->GetSecondChild()->GetVal();

  if (name.empty()) {
    cout << "Invalid expression AST provided!" << endl;
    exit(1);
  }

  ofstream ofs(filename_.c_str());
  // TODO: Stack location
  ofs << "STOREV " << val << endl;

  return;
}
