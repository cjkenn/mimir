#pragma once

#include "../ast.h"
#include "basic_block.h"
#include <memory>

class IrGen {
 public:
  IrGen();
  ~IrGen() {};

  std::shared_ptr<BasicBlock> Gen(AstNodePtr ast);

 private:

};
