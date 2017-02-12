#include <memory>
#include <queue>
#include <vector>
#include <iostream>
#include "ast.h"
#include "ir_block.h"
#include "instruction.h"
#include "ir_gen.h"

IrGen::IrGen() {
  register_count_ = 0;
  lbl_count_ = 0;
  curr_reg_ = "r" + std::to_string(register_count_);
  curr_lbl_ = "lbl" + std::to_string(lbl_count_);
}

std::vector<InstrPtr> IrGen::Gen(AstNodePtr ast) {
  std::vector<InstrPtr> ir;
  AstNodePtr start_ast = ast->GetChildAtIndex(0);

  // Ensure that no ast nodes have been marked as visited.
  ResetAst(start_ast);

  // BFS the ast. The ConvertAstToInstr call may also visit
  // subsequent nodes, so we must make sure to visit those
  // nodes within that call, so we don't visit them again
  // during this BFS.
  std::queue<AstNodePtr> q;
  q.push(start_ast);

  while(!q.empty()) {
    auto node = q.front();
    q.pop();

    std::vector<InstrPtr> instrs = ConvertAstToInstr(node);
    MergeInstrVecs(ir, instrs);

    node->Visit();
    for (auto r : node->GetChildren()) {
      if (!r->IsVisited()) {
	q.push(r);
      }
    }
  }

  return ir;
}

std::vector<InstrPtr> IrGen::ConvertAstToInstr(AstNodePtr ast) {
  std::vector<InstrPtr> instrs;

  if (ast == nullptr) {
    return instrs;
  }

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
  case AstType::EXPR_AST:
    {
      // SEQ and EXPR asts don't really encode any instructions, they
      // are just indicators of program structure. We can skip ahead to the next
      // child and build instructions from there. Both these nodes should be
      // guaranteed to have one child.
      auto next_ast = ast->GetChildAtIndex(0);
      instrs = ConvertAstToInstr(next_ast);
      break;
    }
  default:
    break;
  }

  return instrs;
}

InstrPtr IrGen::VarAstToInstr(AstNodePtr ast) {
  std::pair<std::string, std::string> args(ast->GetText(), curr_reg_);
  InstrPtr ins = std::make_shared<Instruction>(InstructionType::LD_INSTR,
					       args,
					       curr_reg_);
  ins->SetLabel(curr_lbl_);
  AdvanceRegister();

  return ins;
}

InstrPtr IrGen::CstAstToInstr(AstNodePtr ast) {
  std::pair<std::string, std::string> args(std::to_string(ast->GetVal()), curr_reg_);
  InstrPtr ins = std::make_shared<Instruction>(InstructionType::MV_INSTR,
					       args,
					       curr_reg_);
  ins->SetLabel(curr_lbl_);
  AdvanceRegister();

  return ins;
}

InstrPtr IrGen::LtAstToInstr(AstNodePtr ast) {
  InstrPtr ins = std::make_shared<Instruction>();
  ins->SetLabel(curr_lbl_);
  ins->SetType(InstructionType::JMPGT_INSTR);

  // We are jumping to the next label, but we don't want to advance the
  // label yet, because subsequent instructions that are included before
  // the jmp need to be included under the current label.
  ins->SetDest(PeekLabel());

  return ins;
}

// Multiple instructions are needed to convert the if ast
// This function only generates the single block that contains
// the test of the if. Then, the next ast node analyzed will contain
// the next sequence of instructions, and that will be added as the next block.
//
// We expect the ast param to be of type IF_AST or ELSE_AST
std::vector<InstrPtr> IrGen::IfAstToInstr(AstNodePtr ast) {
  std::vector<InstrPtr> v;

  auto if_expr_ast = ast->GetChildAtIndex(0);
  auto left_side = ConvertAstToInstr(if_expr_ast->GetChildAtIndex(0));
  auto right_side = ConvertAstToInstr(if_expr_ast->GetChildAtIndex(1));

  InstrPtr compare = std::make_shared<Instruction>(InstructionType::CMP_INSTR,
						   left_side[0]->GetDest(),
						   right_side[0]->GetDest(),
						   left_side[0]->GetDest());
  compare->SetLabel(curr_lbl_);

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

  // Now we visit the right side of the ast, which contains the instructions
  // to run if the if expression evaluates to true.
  auto if_true_ast = ast->GetChildAtIndex(1);
  auto if_true_instrs = ConvertAstToInstr(if_true_ast);
  MergeInstrVecs(v, if_true_instrs);

  // Visit the nodes in this branch of the ast.
  if_true_ast->VisitNodeAndChildren();

  // Now that we've visited both both relevant branches in this ast,
  // we need to advance the label so the subsequent instructions are
  // jumped to correctly if the if expression evaluates to false.
  AdvanceLabel();

  return v;
}

std::vector<InstrPtr> IrGen::SetAstToInstr(AstNodePtr ast) {
  std::vector<InstrPtr> v;
  if (ast->GetType() != AstType::SET_AST) {
    return v;
  }

  // Set contains the var name in the left child, and the value in the right.
  // We don't use the typical var instruction here since this a set, so we build
  // our own sv instruction here.
  auto right_side = ConvertAstToInstr(ast->GetChildAtIndex(1));
  InstrPtr save = std::make_shared<Instruction>(InstructionType::SV_INSTR,
						curr_reg_);
  save->SetLabel(curr_lbl_);

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
void IrGen::MergeInstrVecs(std::vector<InstrPtr>& v1, std::vector<InstrPtr>& v2) {
  v1.insert(v1.end(), v2.begin(), v2.end());
}

void IrGen::AdvanceRegister() {
  register_count_++;
  curr_reg_ = "r" + std::to_string(register_count_);
}

std::string IrGen::PeekLabel() {
  std::string lbl = "lbl" + std::to_string(lbl_count_+1);
  return lbl;
}

void IrGen::AdvanceLabel() {
  lbl_count_++;
  curr_lbl_ = "lbl" + std::to_string(lbl_count_);
}
