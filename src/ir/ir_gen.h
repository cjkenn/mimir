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
  IrBlockPtr GenIfBlockWhenTrue(AstNodePtr ast);
  IrBlockPtr GenIfBlockWhenFalse(AstNodePtr ast);
  IrBlockPtr GenSeqBlock(AstNodePtr ast);
  Instruction BuildInstrFromAst(AstNodePtr ast);
  std::string GetNextRegister();
  std::string GetCurrRegister();
  std::string GetNextLabel();
  std::string GetCurrLabel();

  int register_count_;
  int lbl_count_;
};
