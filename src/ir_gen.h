#pragma once

#include <memory>
#include <vector>
#include "ast.h"
#include "instruction.h"
#include "ir_block.h"

class IrGen {
 public:
  IrGen();
  ~IrGen() {};

  // Generates a vector of Instruction references by walking the ast.
  // The ast is traversed in a BFS manner, but each since each control
  // structure corresponds to a series of instructions and ast nodes,
  // a dfs-esque search may be performed if several nodes need to be visited
  // to generate instructions.
  std::vector<InstrPtr> Gen(AstNodePtr ast);

 private:
  std::vector<InstrPtr> ConvertAstToInstr(AstNodePtr ast);
  InstrPtr VarAstToInstr(AstNodePtr ast);
  InstrPtr CstAstToInstr(AstNodePtr ast);
  InstrPtr LtAstToInstr(AstNodePtr ast);
  std::vector<InstrPtr> IfAstToInstr(AstNodePtr ast);
  std::vector<InstrPtr> SetAstToInstr(AstNodePtr ast);

  // Vists nodes in the given ast, and sets their Visited value
  // to false, if it is true. This ensures we can traverse an
  // entire tree without accidentally missing nodes, if the
  // tree might have been searched before.
  void ResetAst(AstNodePtr ast);

  // Concatenate two instruction vectors in place.
  void MergeInstrVecs(std::vector<InstrPtr>& v1, std::vector<InstrPtr>& v2);

  // Get the last register used. Useful for statements that want to
  // check the immediately last used, but still allowing other
  // instructions to advance the register.
  std::string PrevRegister();

  // Increment the current register count and set the curr_reg_ field
  // accordingly.
  void AdvanceRegister();

  // Get the value of the next label, but keep the current label the same.
  std::string PeekLabel();

  // Increment the current label count and set the curr_lbl_ field
  // accordingly.
  void AdvanceLabel();

  // Current amount of registers used. Concatenated with 'r' to build a
  // register value for an instruction.
  int register_count_;

  // Current amount of labels used. Concatenated with "lbl" to build
  // a label when needed.
  int lbl_count_;

  // Value of currently active register.
  std::string curr_reg_;

  // Value of currently active label.
  std::string curr_lbl_;
};
