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

IrBlockPtr IrGen::Gen(AstNodePtr ast) {
  // Confirm the ast structure
  if (ast->GetType() != AstType::PROG_AST) {
    // TODO: error handling maybe?
    return nullptr;
  }

  Instruction start_lbl(LBL_INSTR, "start_:");
  IrBlockPtr start_block = std::make_shared<IrBlock>();
  start_block->AddInstruction(start_lbl);

  // IrBlocks contain vectors of instructions,
  // each instruction has a type, 2 args, and a destination.
  // Each IR basic block ends with a jmp or a branch.
  // Each block contains a vector of pointers to other blocks
  // that are adjacent to it (ie. possible code paths)
  // A label is counted as a separate instruction.

  AstNodePtr start_ast = ast->GetChildAtIndex(0);
  switch (start_ast->GetType()) {
  case AstType::IF_AST:
    GenIfBlocks(start_block, start_ast);
  };

  return start_block;
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

  // TODO: ensure label block is created
  // TODO: ensure all instructions are added after label block (recursively?)
  IrBlockPtr else_seq = GenIfBlockWhenFalse(ast);
  root->AddAdjacentBlock(else_seq);
}

IrBlockPtr IrGen::GenIfBlockWhenTrue(AstNodePtr ast) {
  if (ast->GetType() != AstType::IF_AST) {
    // TODO: Error handling
    return nullptr;
  }

  auto seq_ast = ast->GetChildren()[1];
  if (seq_ast->GetType() != AstType::SEQ_AST || seq_ast == nullptr) {
    // TODO: Error handling
    return nullptr;
  }

  IrBlockPtr seq_block = GenSeqBlock(seq_ast);

  return seq_block;
}

IrBlockPtr IrGen::GenIfBlockWhenFalse(AstNodePtr ast) {
  return nullptr;
}

IrBlockPtr IrGen::GenSeqBlock(AstNodePtr ast) {
  if (ast->GetType() != AstType::SEQ_AST) {
    return nullptr;
  }

  auto initial_ast = ast->GetChildren()[0];
  IrBlockPtr seq_block = std::make_shared<IrBlock>();

  // BFS remaining nodes in this branch of ast
  std::queue<AstNodePtr> q;
  q.push(initial_ast);

  while (!q.empty()) {
    auto node = q.front();
    q.pop();

    Instruction instr = BuildInstrFromAst(node);
    seq_block->AddInstruction(instr);
    for (auto adj : node->GetChildren()) {
      q.push(adj);
    }
  }

  return seq_block;
}

Instruction IrGen::BuildInstrFromAst(AstNodePtr ast) {
  Instruction instr;
  std::string first_arg;
  std::string second_arg;
  std::pair<std::string, std::string> args;

  switch(ast->GetType()) {
  case AstType::VAR_AST:
    first_arg = ast->GetText();
    second_arg = GetNextRegister();
    args = std::pair<std::string, std::string>(first_arg, second_arg);
    instr.SetType(InstructionType::LD_INSTR);
    instr.SetArgs(args);
    instr.SetDest(second_arg);
    break;
  case AstType::CST_AST:
    first_arg = std::to_string(ast->GetVal());
    second_arg = GetNextRegister();
    args = std::pair<std::string, std::string>(first_arg, second_arg);
    instr.SetType(InstructionType::MV_INSTR);
    instr.SetArgs(args);
    instr.SetDest(second_arg);
    break;
  case AstType::LT_AST:
    instr.SetType(InstructionType::JMPGT_INSTR);
    instr.SetDest(GetNextLabel());
    break;
  }

  return instr;
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
