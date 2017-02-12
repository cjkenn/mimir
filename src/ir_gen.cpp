#include <memory>
#include <queue>
#include <iostream>
#include "ast.h"
#include "ir_block.h"
#include "instruction.h"
#include "ir_gen.h"

IrGen::IrGen() {
  register_count_ = -1;
  lbl_count_ = -1;
  lbls_used_ = 0;
}

// IrBlocks contain vectors of instructions,
// each instruction has a type, 2 args, and a destination.
// Each IR basic block ends with a jmp or a branch.
// Each block contains a vector of pointers to other blocks
// that are adjacent to it (ie. possible code paths)
// A label is counted as a separate instruction.
IrBlockPtr IrGen::Gen(AstNodePtr ast) {
  // Confirm the ast structure
  if (ast->GetType() != AstType::PROG_AST) {
    return nullptr;
  }

  // 1. BFS on ast
  // 2. For each node, convert the AST to a vector
  //    of instructions (could only be one).
  //    For ast's that convert to multiple instructions,
  //    we may need to mark the children of those nodes
  //    as visited so we don't continue to traverse them.
  //
  // 3. During BFS, if we find a jmp or a label definition,
  //    we end the block there and begin a new one.
  //
  // 4. If a block ends on a jmp, we know that it needs at least
  //    two adjacent blocks, one for if the jmp condition is true and one
  //    other if it is false.
  std::string start_lbl = "start";
  IrBlockPtr start_block = std::make_shared<IrBlock>();
  IrBlockPtr curr_block = start_block;
  start_block->SetLabel(start_lbl);

  AstNodePtr start_ast = ast->GetChildAtIndex(0);

  // Ensure that no ast nodes have been marked as visited.
  ResetAst(start_ast);

  std::queue<AstNodePtr> q;
  q.push(start_ast);

  while(!q.empty()) {
    auto node = q.front();
    q.pop();

    std::vector<Instruction> instrs = ConvertAstToInstr(node);
    curr_block->MergeInstructions(instrs);

    if (LabelRequiredForBlock(node)) {
      curr_block->SetLabel(GetCurrLabel());
      lbls_used_++;
    }

    if (NewBlockRequired(curr_block, node)) {
      IrBlockPtr new_block = std::make_shared<IrBlock>();

      curr_block->AddAdjacentBlock(new_block);
      curr_block = new_block;
    }

    node->Visit();
    for (auto r : node->GetChildren()) {
      if (!r->IsVisited()) {
	q.push(r);
      }
    }
  }

  // TODO: End block to contain clean up instrs if necessary?
  // This also handles a case where we need en empty block
  // with a label to ensure we don't try to jump somewhere
  // that doesn't exist. This can probably be handled better.
  IrBlockPtr end_block = std::make_shared<IrBlock>();
  if (lbls_used_ != (lbl_count_ + 1)) {
    end_block->SetLabel(GetCurrLabel());
    lbls_used_++;
  } else {
    end_block->SetLabel("exit");
  }

  curr_block->AddAdjacentBlock(end_block);

  return start_block;
}
// If the last block ends in a jump, or the current ast node
// is marked with NeedsLabel
bool IrGen::NewBlockRequired(IrBlockPtr ir, AstNodePtr ast) {
  if (ast->NeedsIrLabel()) {
    return true;
  }

  auto instrs = ir->GetInstructions();
  if (instrs.empty()) {
    return false;
  }

  auto last_instr = instrs.back();
  return last_instr.IsJmp();
}

bool IrGen::LabelRequiredForBlock(AstNodePtr ast) {
  // TODO: This isn't a good way to determine this, as it makes unconditional
  // jmps unusable in other situations.
  //
  // Instructions need to be labelled with "is last instr in block", and
  // that flag should be read when trying to determine if a new block
  // is required.
  // Need a way to determine when a new block needs a label, and what that
  // label should be.
  // New blocks: right after jmps, and right before a label is needed
  // A label is needed in specific circumstances, which we should determine
  // based on context derived from the ast.
  // ie. is the parent and if_ast and we are in the else branch? then a label
  // is needed
  // This can be encoded in the ast during parsing? Or derived from the
  // ast during ir gen, but probably easier if we add flags to the ast node
  // itself...
  // It may also be possible to perform a distinct pass of the ast (or even
  // the ir) and flag blocks that need labels?
  // Harder to make this label testing general, like the ir construction is
  return ast->NeedsIrLabel();
}

std::vector<Instruction> IrGen::ConvertAstToInstr(AstNodePtr ast) {
  std::vector<Instruction> instrs;

  switch(ast->GetType()) {
  case AstType::VAR_AST:
    instrs.push_back(VarAstToInstr(ast));
    break;
  case AstType::CST_AST:
    instrs.push_back(CstAstToInstr(ast));
    break;
  case AstType::ELSE_AST:
  case AstType::IF_AST:
    {
      auto if_instrs = IfAstToInstr(ast);
      MergeInstrVecs(instrs, if_instrs);
      break;
    }
  case AstType::SET_AST:
    {
      auto set_instrs = SetAstToInstr(ast);
      MergeInstrVecs(instrs, set_instrs);
      break;
    }
  case AstType::LT_AST:
    instrs.push_back(LtAstToInstr(ast));
    break;
  case AstType::SEQ_AST:
    {
      // TODO: Parse an entire sequence of instrs into a vector
      // visit each child ast node in the seq, call convert on them
      break;
    }
  case AstType::EXPR_AST:
  default:
    // We don't really care about these ast types here, because the real
    // instructions are located in children ast nodes. So we skip them.
    break;
  }

  return instrs;
}

Instruction IrGen::VarAstToInstr(AstNodePtr ast) {
  std::pair<std::string, std::string> args(ast->GetText(), GetNextRegister());
  Instruction ins(InstructionType::LD_INSTR,
		  args,
		  GetCurrRegister());
  return ins;
}

Instruction IrGen::CstAstToInstr(AstNodePtr ast) {
  std::pair<std::string, std::string> args(std::to_string(ast->GetVal()), GetNextRegister());
  Instruction ins(InstructionType::MV_INSTR,
		  args,
		  GetCurrRegister());
  return ins;
}

Instruction IrGen::LtAstToInstr(AstNodePtr ast) {
  Instruction ins(InstructionType::JMPGT_INSTR,
		  GetNextLabel());
  return ins;
}

// Multiple instructions are needed to convert the if ast
// This function only generates the single block that contains
// the test of the if. Then, the next ast node analyzed will contain
// the next sequence of instructions, and that will be added as the next block.
//
// We expect the ast param to be of type IF_AST or ELSE_AST
std::vector<Instruction> IrGen::IfAstToInstr(AstNodePtr ast) {
  std::vector<Instruction> v;

  auto if_expr_ast = ast->GetChildAtIndex(0);
  auto left_side = ConvertAstToInstr(if_expr_ast->GetChildAtIndex(0));
  auto right_side = ConvertAstToInstr(if_expr_ast->GetChildAtIndex(1));

  Instruction compare(InstructionType::CMP_INSTR,
		      left_side[0].GetDest(),
		      right_side[0].GetDest(),
		      left_side[0].GetDest());

  auto jmp = ConvertAstToInstr(if_expr_ast);

  // The order of instructions matters here: we need to add the comparison
  // arguments first, then the cmp, then the relevant jmp.
  MergeInstrVecs(v, left_side);
  MergeInstrVecs(v, right_side);
  v.push_back(compare);
  MergeInstrVecs(v, jmp);

  // We've now visited the if expr ast node, as well as each node
  // in the left side branch of the if. We should mark all those nodes
  // as visited to ensure we don't try to add instructions for them
  // later on (as we continue to traverse the tree)
  if_expr_ast->VisitNodeAndChildren();

  return v;
}

std::vector<Instruction> IrGen::SetAstToInstr(AstNodePtr ast) {
  std::vector<Instruction> v;
  if (ast->GetType() != AstType::SET_AST) {
    return v;
  }

  // Set contains the var name in the left child, and the value in the right.
  // We don't use the typical var instruction here since this a set, so we build
  // our own sv instruction here.
  auto right_side = ConvertAstToInstr(ast->GetChildAtIndex(1));
  Instruction save(InstructionType::SV_INSTR,
		   GetCurrRegister());

  MergeInstrVecs(v, right_side);
  v.push_back(save);

  // Ensure we visit the set arguments here so we don't search them
  // multiple times.
  ast->VisitNodeAndChildren();

  return v;
}

void IrGen::ResetAst(AstNodePtr ast) {
  std::queue<AstNodePtr> q;
  q.push(ast);

  while (!q.empty()) {
    auto node = q.front();
    q.pop();
    node->SetVisited(false);
    for (auto r : node->GetChildren()) {
      // If we have a visited node, make sure we reset it to false.
      if (r->IsVisited()) {
	q.push(r);
      }
    }
  }
}

// Puts the contents of v2 into v1.
void IrGen::MergeInstrVecs(std::vector<Instruction>& v1, std::vector<Instruction>& v2) {
  v1.insert(v1.end(), v2.begin(), v2.end());
}

std::string IrGen::GetNextRegister() {
  register_count_++;
  std::string reg = "r" + std::to_string(register_count_);
  return reg;
}

std::string IrGen::GetCurrRegister() {
  std::string reg = "r" + std::to_string(register_count_);
  return reg;
}

std::string IrGen::GetNextLabel() {
  lbl_count_++;
  std::string lbl = "lbl" + std::to_string(lbl_count_);
  return lbl;
}

std::string IrGen::GetCurrLabel() {
  std::string lbl = "lbl" + std::to_string(lbl_count_);
  return lbl;
}
