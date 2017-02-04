#include <memory>
#include <queue>
#include "../ast.h"
#include "ir_block.h"
#include "instruction.h"
#include "ir_gen.h"

IrGen::IrGen() {
  register_count_ = -1;
  lbl_count_ = -1;
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
    // TODO: error handling maybe?
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
  Instruction start_lbl(LBL_INSTR, "start_:");
  IrBlockPtr start_block = std::make_shared<IrBlock>();
  IrBlockPtr curr_block = start_block;
  start_block->AddInstruction(start_lbl);

  AstNodePtr start_ast = ast->GetChildAtIndex(0);

  // Ensure that no ast nodes have been marked as visited.
  ResetAst(start_ast);

  std::queue<AstNodePtr> q;
  q.push(start_ast);

  while(!q.empty()) {
    auto node = q.front();
    q.pop();

    std::vector<Instruction> instrs = ConvertAstToInstruction(node);
    curr_block->MergeInstructions(instrs);
    if (NewBlockRequired(curr_block)) {
      IrBlockPtr new_block;
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

  return start_block;
}

std::vector<Instruction> IrGen::ConvertAstToInstruction(AstNodePtr ast) {
  std::vector<Instruction> instrs;

  // TODO: How to build instruction from SET ast?
  // TODO: How to build multiple instructions from single ast?
  switch(ast->GetType()) {
  case AstType::VAR_AST:
    instrs.push_back(VarAstToInstr(ast));
    break;
  case AstType::CST_AST:
    instrs.push_back(CstAstToInstr(ast));
    break;
  case AstType::IF_AST:
    {
      auto if_instrs = IfAstToInstr(ast);
      instrs.insert(instrs.end(), if_instrs.begin(), if_instrs.end());
      break;
    }
  case AstType::ELSE_AST:
    break;
  case AstType::LT_AST:
    // instr.SetType(InstructionType::JMPGT_INSTR);
    // instr.SetDest(GetNextLabel());
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

std::vector<Instruction> IrGen::IfAstToInstr(AstNodePtr ast) {

  return 0;
}

// Expects the ast passed in to be of type IF_AST
void IrGen::GenIfBlocks(IrBlockPtr root, AstNodePtr ast) {
  if (ast->GetType() != AstType::IF_AST) {
    return;
  }

  auto if_expr_ast = ast->GetChildren()[0];
  auto left_side_ast = if_expr_ast->GetChildren()[0];
  Instruction left_side_cmp = BuildInstrFromAst(left_side_ast);

  auto right_side_ast = if_expr_ast->GetChildren()[1];
  Instruction right_side_cmp = BuildInstrFromAst(right_side_ast);

  // Comparison will take the two destination registers as args,
  // and return an expected result in the first register.
  Instruction comparison(InstructionType::CMP_INSTR,
			 left_side_cmp.GetDest(),
			 right_side_cmp.GetDest(),
			 left_side_cmp.GetDest());

  root->AddInstruction(left_side_cmp);
  root->AddInstruction(right_side_cmp);
  root->AddInstruction(comparison);

  Instruction conditional_jmp = BuildInstrFromAst(if_expr_ast);
  root->AddInstruction(conditional_jmp);

  // We end the root block with the branching instruction.
  // For an if statement, this block can either move to the
  // next block if the conditional is true, or skip to the jmp
  // label if it is false. We build both those blocks, and add them
  // to the adjacent list for the root block.
  IrBlockPtr if_seq = GenIfBlockWhenTrue(ast);
  root->AddAdjacentBlock(if_seq);

  IrBlockPtr else_seq = GenIfBlockWhenFalse(ast);
  root->AddAdjacentBlock(else_seq);
}

IrBlockPtr IrGen::GenIfBlockWhenTrue(AstNodePtr ast) {
  if (ast->GetType() != AstType::IF_AST) {
    return nullptr;
  }

  auto seq_ast = ast->GetChildren()[1];
  if (seq_ast->GetType() != AstType::SEQ_AST || seq_ast == nullptr) {
    return nullptr;
  }

  IrBlockPtr seq_block = GenSeqBlock(seq_ast);
  Instruction unconditional_jmp(InstructionType::JMP_INSTR, GetCurrLabel());
  seq_block->AddInstruction(unconditional_jmp);

  return seq_block;
}

IrBlockPtr IrGen::GenIfBlockWhenFalse(AstNodePtr ast) {
  // TODO: Which ast to use for this call?
  IrBlockPtr seq_block = GenSeqBlock(ast);

  Instruction cont_lbl(InstructionType::LBL_INSTR, GetCurrLabel());
  seq_block->PrependInstruction(seq_block);

  return seq_block;
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
