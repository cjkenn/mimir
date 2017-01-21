#pragma once

#include <iostream>
#include <fstream>
#include "ast.h"

enum GenTarget {
  TYR_GEN
};

class Gen {
 public:
  Gen(AstNodePtr program, std::string filename);
  ~Gen() {};

  void Generate(GenTarget target);

 private:
  void GenerateTyr();
  void EmitVar(AstNodePtr node);
  void EmitExpr(AstNodePtr node);

  std::string filename_;
  AstNodePtr program_;
};
