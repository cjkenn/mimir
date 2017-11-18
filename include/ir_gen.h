#pragma once

#include <memory>
#include <vector>
#include "ast.h"
#include "ir_instr.h"

namespace mimir {
// IrGen is used to create a vector of IR instructions that can be used in control flow analysis
// and make it a bit easier to generate assembly code, when compared to generating
// right from an ast. For more details on what the ir contains, see ir_instr.h
class IrGen {
 public:
  IrGen();
  ~IrGen() {};

  // Generates a vector of Instruction references by walking the ast.
  // The ast is traversed in a BFS manner, but since each control
  // structure corresponds to a series of instructions and ast nodes,
  // a dfs-esque search may be performed if several nodes need to be visited
  // to generate instructions.
  std::vector<IrInstrPtr> Gen(AstNodePtr ast);
  int GetRegCount() const { return register_count_; }

 private:
  // Converts an ast branch to a series of instructions. Some ast's
  // will only require a single instructions, but many require more than
  // one, so this returns a vector.
  std::vector<IrInstrPtr> ConvertAstToInstr(AstNodePtr ast);

  IrInstrPtr VarAstToInstr(AstNodePtr ast);
  IrInstrPtr CstAstToInstr(AstNodePtr ast);
  IrInstrPtr TestAstToInstr(AstNodePtr ast);

  std::vector<IrInstrPtr> IfElseAstToInstr(AstNodePtr ast);
  std::vector<IrInstrPtr> WhileAstToInstr(AstNodePtr ast);
  std::vector<IrInstrPtr> SetAstToInstr(AstNodePtr ast);
  std::vector<IrInstrPtr> FuncAstToInstr(AstNodePtr ast);
  std::vector<IrInstrPtr> FuncCallAstToInstr(AstNodePtr ast);
  std::vector<IrInstrPtr> ParamsAstToInstr(AstNodePtr ast);
  std::vector<IrInstrPtr> BinOpAstToInstr(AstNodePtr ast);

  // The parenthetical expressions that are used the test
  // whether we enter ifs or whiles can be generated in the
  // same manner, so we extract that functionality to this method.
  std::vector<IrInstrPtr> ComparisonAstToInstr(AstNodePtr ast);

  // Maps the ast types for binary operations to the
  // instruction types for the same ops. This function exists
  // so we can use the same BinOpAstToInstr method for every
  // binary op, instead of a different method for each op.
  IrInstrType GetBinOpInstrTypeFromAst(AstNodePtr ast);

  // Used to determine the correct instruction type for comparison
  // test asts. The results are not a 1-1 mapping, as we want
  // the opposite test instruction to generate the correct ir code.
  // For example, LT_AST maps to GT_INSTR.
  IrInstrType GetJmpInstrTypeFromAst(AstNodePtr ast);

  // Vists nodes in the given ast, and sets their Visited value
  // to false, if it is true. This ensures we can traverse an
  // entire tree without accidentally missing nodes, if the
  // tree might have been searched before.
  void ResetAst(AstNodePtr ast);

  // Concatenate two instruction vectors in place.
  void MergeInstrVecs(std::vector<IrInstrPtr>& v1, std::vector<IrInstrPtr>& v2);

  // Get the last register used. Useful for statements that want to
  // check the immediately last used, but still allowing other
  // instructions to advance the register.
  const std::string PrevRegister();

  // Increment the current register count and set the curr_reg_ field
  // accordingly.
  void AdvanceRegister();

  // Get the value of the next label, but keep the current label the same.
  const std::string PeekLabel();

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
}
