#include <memory>
#include <queue>
#include <vector>
#include <iostream>
#include <assert.h>
#include <utility>
#include "ast.h"
#include "ir_instr.h"
#include "ir_gen.h"

using namespace mimir;

IrGen::IrGen() {
  register_count_ = 0;
  lbl_count_ = 0;
  curr_reg_ = "r" + std::to_string(register_count_);
  curr_lbl_ = "lbl" + std::to_string(lbl_count_);
}

std::vector<IrInstrPtr> IrGen::Gen(AstNodePtr ast) {
  std::vector<IrInstrPtr> ir;

  // Ensure that no ast nodes have been marked as visited.
  ResetAst(ast);

  // BFS the ast. The ConvertAstToInstr call may also visit
  // subsequent nodes, so we must make sure to visit those
  // nodes within that call, so we don't visit them again
  // during this BFS.
  std::queue<AstNodePtr> q;
  q.push(ast);

  while(!q.empty()) {
    auto node = q.front();
    q.pop();

    std::vector<IrInstrPtr> instrs = ConvertAstToInstr(node);
    MergeInstrVecs(ir, instrs);

    node->SetVisited(true);
    for (auto r : node->GetChildren()) {
      if (!r->IsVisited()) {
	q.push(r);
      }
    }
  }

  // If a jmp is needed to a label that has no instructions under it,
  // this label might not be added to the ir, so we do that here by
  // creating a dummy instruction to ensure the label exists.
  auto dummy = std::make_shared<IrInstr>(IrInstrType::NOP_INSTR);
  dummy->SetLabel(curr_lbl_);
  ir.push_back(dummy);

  return ir;
}

std::vector<IrInstrPtr> IrGen::ConvertAstToInstr(AstNodePtr ast) {
  std::vector<IrInstrPtr> instrs;

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
      auto if_instrs = IfElseAstToInstr(ast);
      MergeInstrVecs(instrs, if_instrs);
      break;
    }
  case AstType::WHILE_AST:
    {
      auto while_instrs = WhileAstToInstr(ast);
      MergeInstrVecs(instrs, while_instrs);
      break;
    }
  case AstType::SET_AST:
    {
      auto set_instrs = SetAstToInstr(ast);
      MergeInstrVecs(instrs, set_instrs);
      break;
    }
  case AstType::FUNC_AST:
    {
      auto func_instrs = FuncAstToInstr(ast);
      MergeInstrVecs(instrs, func_instrs);
      break;
    }
  case AstType::FUNC_CALL_AST:
    {
      auto func_call_instrs = FuncCallAstToInstr(ast);
      MergeInstrVecs(instrs, func_call_instrs);
      break;
    }
  case AstType::PARAMS_AST:
    {
      auto params_instrs = ParamsAstToInstr(ast);
      MergeInstrVecs(instrs, params_instrs);
      break;
    }
  case AstType::ADD_AST:
  case AstType::SUB_AST:
  case AstType::MUL_AST:
  case AstType::DIV_AST:
  case AstType::MOD_AST:
    {
      auto add_instrs = BinOpAstToInstr(ast);
      MergeInstrVecs(instrs, add_instrs);
      break;
    }
  case AstType::LT_AST:
  case AstType::GT_AST:
  case AstType::GTE_AST:
  case AstType::LTE_AST:
  case AstType::EQEQ_AST:
  case AstType::NEQ_AST:
    instrs.push_back(TestAstToInstr(ast));
    break;
  case AstType::SEQ_AST:
    {
      // We need to convert each child ast in the sequence to an ir instruction,
      // so we recursively call convert for all children.
      for (auto child : ast->GetChildren()) {
	auto seq_instrs = ConvertAstToInstr(child);
	MergeInstrVecs(instrs, seq_instrs);
      }
      break;
    }
  case AstType::EXPR_AST:
    {
      // EXPR asts don't really encode any instructions, they
      // are just indicators of program structure. We can skip ahead to the next
      // child and build instructions from there. We should be guaranteed to have
      // one child here.
      assert(ast->GetChildren().size() > 0);
      auto next_ast = ast->GetChildAtIndex(0);
      instrs = ConvertAstToInstr(next_ast);
      break;
    }
  case AstType::PROG_AST:
  case AstType::EMPTY_AST:
  case AstType::EOF_AST:
  default:
    break;
  }

  return instrs;
}

IrInstrPtr IrGen::VarAstToInstr(AstNodePtr ast) {
  assert(ast->GetType() == AstType::VAR_AST);
  const std::pair<std::string, std::string> args(ast->GetText(), curr_reg_);
  IrInstrPtr ins = std::make_shared<IrInstr>(IrInstrType::LD_INSTR,
					     args,
					     curr_reg_);
  ins->SetLabel(curr_lbl_);
  AdvanceRegister();

  return ins;
}

IrInstrPtr IrGen::CstAstToInstr(AstNodePtr ast) {
  assert(ast->GetType() == AstType::CST_AST);
  const std::pair<std::string, std::string> args(std::to_string(ast->GetVal()), curr_reg_);
  IrInstrPtr ins = std::make_shared<IrInstr>(IrInstrType::MV_INSTR,
					     args,
					     curr_reg_);
  ins->SetLabel(curr_lbl_);
  AdvanceRegister();

  return ins;
}

IrInstrPtr IrGen::TestAstToInstr(AstNodePtr ast) {
  IrInstrPtr ins = std::make_shared<IrInstr>();
  ins->SetLabel(curr_lbl_);
  ins->SetType(GetJmpInstrTypeFromAst(ast));

  // We are jumping to the next label, but we don't want to advance the
  // label yet, because subsequent instructions that are included before
  // the jmp need to be included under the current label.
  ins->SetDest(PeekLabel());

  return ins;
}

// We expect the ast param to be of type IF_AST or ELSE_AST
std::vector<IrInstrPtr> IrGen::IfElseAstToInstr(AstNodePtr ast) {
  std::vector<IrInstrPtr> v;

  auto expr_test = ComparisonAstToInstr(ast);
  MergeInstrVecs(v, expr_test);

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

  // Write instructions for the code that follows in the else block,
  // if that is the type of ast we were passed in.
  if (ast->GetType() == AstType::ELSE_AST) {
    auto else_ast = ast->GetChildAtIndex(2);
    auto else_instrs = ConvertAstToInstr(else_ast);
    MergeInstrVecs(v, else_instrs);

    else_ast->VisitNodeAndChildren();
  }

  return v;
}

std::vector<IrInstrPtr> IrGen::WhileAstToInstr(AstNodePtr ast) {
  assert(ast->GetType() == AstType::WHILE_AST);
  std::vector<IrInstrPtr> v;

  auto expr_test = ComparisonAstToInstr(ast);
  MergeInstrVecs(v, expr_test);

  auto while_true_ast = ast->GetChildAtIndex(1);
  auto while_true_instrs = ConvertAstToInstr(while_true_ast);
  IrInstrPtr always_jmp = std::make_shared<IrInstr>(IrInstrType::JMP_INSTR);
  always_jmp->SetLabel(curr_lbl_);
  always_jmp->SetDest(curr_lbl_);

  // Order should be maintained here. The unconditional jump instruction
  // should be added last.
  MergeInstrVecs(v, while_true_instrs);
  v.push_back(always_jmp);

  while_true_ast->VisitNodeAndChildren();

  // Now that we've visited both both relevant branches in this ast,
  // we need to advance the label so the subsequent instructions are
  // jumped to correctly if the while expression evaluates to false.
  AdvanceLabel();

  return v;
}

std::vector<IrInstrPtr> IrGen::ComparisonAstToInstr(AstNodePtr ast) {
  std::vector<IrInstrPtr> v;

  auto expr_ast = ast->GetChildAtIndex(0);
  auto left_side = ConvertAstToInstr(expr_ast->GetChildAtIndex(0));
  auto right_side = ConvertAstToInstr(expr_ast->GetChildAtIndex(1));

  IrInstrPtr compare = std::make_shared<IrInstr>(IrInstrType::CMP_INSTR,
						 left_side[0]->GetDest(),
						 right_side[0]->GetDest(),
						 left_side[0]->GetDest());
  compare->SetLabel(curr_lbl_);

  auto jmp = ConvertAstToInstr(expr_ast);

  MergeInstrVecs(v, left_side);
  MergeInstrVecs(v, right_side);
  v.push_back(compare);
  MergeInstrVecs(v, jmp);

  expr_ast->VisitNodeAndChildren();

  return v;
}

std::vector<IrInstrPtr> IrGen::SetAstToInstr(AstNodePtr ast) {
  assert(ast->GetType() == AstType::SET_AST);
  std::vector<IrInstrPtr> v;

  // Set contains the var name in the left child, and the value in the right.
  // We don't use the typical var instruction here since this a set, so we build
  // our own sv instruction here.
  auto right_side = ConvertAstToInstr(ast->GetChildAtIndex(1));
  const std::string name = ast->GetChildAtIndex(0)->GetText();
  IrInstrPtr save = std::make_shared<IrInstr>(IrInstrType::SV_INSTR,
					      PrevRegister(),
					      name,
					      name);
  save->SetLabel(curr_lbl_);

  MergeInstrVecs(v, right_side);
  v.push_back(save);

  // Ensure we visit the set arguments here so we don't search them
  // multiple times.
  ast->VisitNodeAndChildren();

  return v;
}

std::vector<IrInstrPtr> IrGen::FuncAstToInstr(AstNodePtr ast) {
  assert(ast->GetType() == AstType::FUNC_AST);
  std::vector<IrInstrPtr> v;
  auto func_name = ast->GetText();

  // Generate label with function name. We use a custom instruction
  // called func_enter, which will expand to the proper calling sequence
  // when we convert to a backend assembly. For now we don't really care.
  IrInstrPtr func_enter = std::make_shared<IrInstr>(IrInstrType::FUNC_ENTER_INSTR);
  func_enter->SetLabel(func_name);
  v.push_back(func_enter);

  // The left side of this ast represents the function params. We don't really need
  // to do anything with those here, since when the function is called the params
  // are pushed on to the stack and saved using the func_enter instr above.
  // TODO: Will there be any scoping issues here?
  auto func_body = ConvertAstToInstr(ast->GetChildAtIndex(1));

  for (auto instr : func_body) {
    instr->SetLabel(func_name);
  }

  MergeInstrVecs(v, func_body);

  IrInstrPtr func_exit = std::make_shared<IrInstr>(IrInstrType::FUNC_EXIT_INSTR);
  func_exit->SetLabel(func_name);
  v.push_back(func_exit);

  ast->VisitNodeAndChildren();

  return v;
}

std::vector<IrInstrPtr> IrGen::FuncCallAstToInstr(AstNodePtr ast) {
  assert(ast->GetType() == AstType::FUNC_CALL_AST);

  auto param_instrs = ConvertAstToInstr(ast->GetChildAtIndex(0));
  auto const func_name = ast->GetText();

  IrInstrPtr func_call = std::make_shared<IrInstr>(IrInstrType::FUNC_CALL_INSTR);
  func_call->SetArgs(std::make_pair(func_name, ""));
  AdvanceLabel();
  func_call->SetLabel(curr_lbl_);

  // We need to reset the labels for params so that we have them in the same block
  // as this function call.
  for (auto param : param_instrs) {
    param->SetLabel(curr_lbl_);
  }
  param_instrs.push_back(func_call);

  ast->VisitNodeAndChildren();

  return param_instrs;
}

std::vector<IrInstrPtr> IrGen::ParamsAstToInstr(AstNodePtr ast) {
  assert(ast->GetType() == AstType::PARAMS_AST);
  std::vector<IrInstrPtr> v;

  for (auto param_ast : ast->GetChildren()) {
    IrInstrPtr param = std::make_shared<IrInstr>(IrInstrType::FUNC_PARAM_INSTR);
    param->SetLabel(curr_lbl_);

    if (param_ast->GetType() == AstType::VAR_AST) {
      param->SetArgs(std::make_pair(param_ast->GetText(), ""));
    } else if (param_ast->GetType() == AstType::CST_AST) {
      param->SetArgs(std::make_pair(std::to_string(param_ast->GetVal()), ""));
    }
    v.insert(v.begin(), param);
  }

  ast->VisitNodeAndChildren();

  return v;
}

std::vector<IrInstrPtr> IrGen::BinOpAstToInstr(AstNodePtr ast) {
  std::vector<IrInstrPtr> v;
  auto op_instr_type = GetBinOpInstrTypeFromAst(ast);
  auto left_side = ConvertAstToInstr(ast->GetChildAtIndex(0));

  // Save the register this is in, so we can use it later when we
  // add the actual ADD instruction.
  const std::string first_add_arg = PrevRegister();
  auto right_side = ConvertAstToInstr(ast->GetChildAtIndex(1));

  const std::pair<std::string, std::string> args(first_add_arg, PrevRegister());
  IrInstrPtr add = std::make_shared<IrInstr>(op_instr_type,
					     args,
					     PrevRegister());
  add->SetLabel(curr_lbl_);

  MergeInstrVecs(v, left_side);
  MergeInstrVecs(v, right_side);
  v.push_back(add);

  ast->VisitNodeAndChildren();

  return v;
}

IrInstrType IrGen::GetBinOpInstrTypeFromAst(AstNodePtr ast) {
  assert(ast->IsBinOp() == true);
  auto ast_type = ast->GetType();

  switch(ast_type) {
  case AstType::ADD_AST:
    return IrInstrType::ADD_INSTR;
  case AstType::SUB_AST:
    return IrInstrType::SUB_INSTR;
  case AstType::MUL_AST:
    return IrInstrType::MUL_INSTR;
  case AstType::DIV_AST:
    return IrInstrType::DIV_INSTR;
  case AstType::MOD_AST:
    return IrInstrType::MOD_INSTR;
  default:
    return IrInstrType::NOP_INSTR;
  }
}

IrInstrType IrGen::GetJmpInstrTypeFromAst(AstNodePtr ast) {
  assert(ast->IsCmp() == true);
  auto ast_type = ast->GetType();

  switch(ast_type) {
  case AstType::LT_AST:
    return IrInstrType::JMPGT_INSTR;
  case AstType::GT_AST:
    return IrInstrType::JMPLT_INSTR;
  case AstType::LTE_AST:
    return IrInstrType::JMPGTE_INSTR;
  case AstType::GTE_AST:
    return IrInstrType::JMPLTE_INSTR;
  case AstType::EQEQ_AST:
    return IrInstrType::JMPNEQ_INSTR;
  case AstType::NEQ_AST:
    return IrInstrType::JMPEQ_INSTR;
  default:
    return IrInstrType::NOP_INSTR;
  }
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
void IrGen::MergeInstrVecs(std::vector<IrInstrPtr>& v1, std::vector<IrInstrPtr>& v2) {
  v1.insert(v1.end(), v2.begin(), v2.end());
}

const std::string IrGen::PrevRegister() {
  const std::string reg = "r" + std::to_string(register_count_ - 1);
  return reg;
}

void IrGen::AdvanceRegister() {
  register_count_++;
  curr_reg_ = "r" + std::to_string(register_count_);
}

const std::string IrGen::PeekLabel() {
  const std::string lbl = "lbl" + std::to_string(lbl_count_+1);
  return lbl;
}

void IrGen::AdvanceLabel() {
  lbl_count_++;
  curr_lbl_ = "lbl" + std::to_string(lbl_count_);
}
