#pragma once

#include "../ast.h"
#include "basic_block.h"
#include <memory>

class IrGen {
 public:
  IrGen();
  ~IrGen() {};

  BasicBlockPtr Gen(AstNodePtr ast);

 private:
  BasicBlockPtr GetBlock(AstNodePtr ast);
  int reg_count_;
};
