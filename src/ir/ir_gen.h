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
  std::vector<Instruction> ConvertAstToInstruction(AstNodePtr ast);
  bool NewBlockRequired(IrBlockPtr ir);

  // TODO: Likely not needed
  void GenIfBlocks(IrBlockPtr root, AstNodePtr ast);
  IrBlockPtr GenIfBlockWhenTrue(AstNodePtr ast);
  IrBlockPtr GenIfBlockWhenFalse(AstNodePtr ast);

  Instruction VarAstToInstr(AstNodePtr ast);
  Instruction CstAstToInstr(AstNodePtr ast);
  std::vector<Instruction> IfAstToInstr(AstNodePtr ast);

  void ResetAst(AstNodePtr ast);
  std::string GetNextRegister();
  std::string GetCurrRegister();
  std::string GetNextLabel();
  std::string GetCurrLabel();

  int register_count_;
  int lbl_count_;
};
