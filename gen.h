#pragma once

#include <iostream>
#include <fstream>
#include "ast.h"

enum GenTarget {
  TYR_GEN
};

class Gen {
 public:
  Gen(AstNode *program, std::string filename);
  ~Gen();

  void Generate(GenTarget target);

 private:
  void GenerateTyr();
  void EmitVar(AstNode *node);
  void EmitExpr(AstNode *node);

  std::string filename_;
  AstNode *program_;
};
