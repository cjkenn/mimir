#pragma once

#include "../ast.h"
#include "ir_block.h"
#include <memory>

class IrGen {
 public:
  IrGen();
  ~IrGen() {};

  IrBlockPtr Gen(AstNodePtr ast);

 private:
  void GenIfBlocks(IrBlockPtr root, AstNodePtr ast);
  Instruction BuildInstrFromAst(AstNodePtr ast);
  std::string GetNextRegister();
  std::string GetNextLabel();

  int register_count_;
  int lbl_count_;
};
