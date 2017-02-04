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
  std::vector<Instruction> ConvertAstToInstr(AstNodePtr ast);
  bool NewBlockRequired(IrBlockPtr ir);
  Instruction VarAstToInstr(AstNodePtr ast);
  Instruction CstAstToInstr(AstNodePtr ast);
  Instruction LtAstToInstr(AstNodePtr ast);
  std::vector<Instruction> IfAstToInstr(AstNodePtr ast);
  std::vector<Instruction> SeqAstToInstr(AstNodePtr ast);

  void ResetAst(AstNodePtr ast);
  void MergeInstrVecs(std::vector<Instruction> v1, std::vector<Instruction> v2);
  std::string GetNextRegister();
  std::string GetCurrRegister();
  std::string GetNextLabel();
  std::string GetCurrLabel();

  int register_count_;
  int lbl_count_;
};
