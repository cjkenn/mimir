#include <unordered_map>
#include <utility>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <queue>
#include "local_optimizer.h"

LocalOptimizer::LocalOptimizer() {
  val_count_ = 0;
}

void LocalOptimizer::OptimizeBlock(CfgNodePtr& block) {
  val_count_ = 0;
  val_map_.clear();
  op_map_.clear();
  const auto instrs = block->GetInstrs();

  for (unsigned int i = 0; i < instrs.size(); i++) {
    const auto instr = instrs[i];
    // Skip everything but saves, loads, and binops
    if (!IsInstrLvnValid(instr)) {
      continue;
    }

    // Check if an operation has a constant value. If it does, we can
    // mark the previous MV instructions as redundant, and flag the current op
    // as constant. Later, when we alter the instrs vector, we can change
    // constant operations to MVs.
    CheckAndMarkConstantOp(instrs, i);
    CheckAndMarkIdentities(instrs, i);

    // Bounds check, because if this is the last instruction we won't be
    // concerned with lvn below.
    if (i == instrs.size() - 1) {
      break;
    }

    const int first_val = GetLvnForFirstArg(instr);
    const int second_val = GetLvnForSecondArg(instr, first_val);
    const std::string key = BuildLvnMapKey(instr, first_val, second_val);
    auto op_map_result = op_map_.find(key);

    if (op_map_result == op_map_.end()) {
      // If the instruction key doesn't exist already, we need to decide if
      // we want to store it. If the value of the operation is never stored in
      // a var, there is nothing to save (because there's nothing to load from
      // later on).
      if (instrs[i+1]->GetType() == IrInstrType::SV_INSTR) {
	const std::string dest = instrs[i+1]->GetDest();
	op_map_.insert(std::pair<std::string, std::string>(key, dest));
      }
    } else {
      // If we've found the calculation, we need to check if we plan to store it.
      // To do that, we look at the next instruction and see if it's a SV
      if (instrs[i+1]->GetType() == IrInstrType::SV_INSTR) {
	// We do plan to store it. Now we can remove the original operation,
	// and replace the sv instruction destination with the value from
	// the op_map table
	const std::string new_input = op_map_result->second;
	const std::string dest = instrs[i+1]->GetDest();

	// TODO: Should we load the stored calc value into a register first? (ie.
	// how should the sv instr actually work)
        instrs[i+1]->SetArgs(std::pair<std::string, std::string>(new_input, dest));
	instr->SetRedundant(true);
	MarkPreviousLdInstrs(instrs, i);
      }
    }
  }

  // New we iterate through the instrs and skip any that are marked as redundant,
  // as well as fold constant operations by converting them to moves.
  std::vector<IrInstrPtr> new_instrs;

  for (auto instr : instrs) {
    if (!instr->IsRedundant()) {

      if (instr->IsConstantOp()) {
	instr->ConvertConstant();
      }

      new_instrs.push_back(instr);
    }
  }

  block->SetInstrs(new_instrs);
}

void LocalOptimizer::OptimizeEntireBranch(CfgNodePtr& root) {
  std::queue<CfgNodePtr> q;
  q.push(root);

  while (!q.empty()) {
    auto block = q.front();
    q.pop();

    block->SetVisited(true);
    OptimizeBlock(block);

    for (auto adj_block : block->GetAdj()) {
      if (!adj_block->GetVisited()) {
	q.push(adj_block);
      }
    }
  }
}

bool LocalOptimizer::IsInstrLvnValid(const IrInstrPtr& instr) {
  return (instr->IsBinOp() ||
	  instr->GetType() == IrInstrType::SV_INSTR ||
	  instr->GetType() == IrInstrType::LD_INSTR ||
	  instr->GetType() == IrInstrType::MV_INSTR);
}

void LocalOptimizer::CheckAndMarkConstantOp(const std::vector<IrInstrPtr>& instrs, const int i) {
  if (!instrs[i]->IsBinOp() || i < 2) {
    return;
  }

  if (instrs[i-1]->GetType() == IrInstrType::MV_INSTR &&
      instrs[i-2]->GetType() == IrInstrType::MV_INSTR) {
    const int first_cst = std::stoi(instrs[i-2]->GetArgs().first);
    const int second_cst = std::stoi(instrs[i-1]->GetArgs().first);
    const int val = EvalConstantOp(instrs[i], first_cst, second_cst);

    instrs[i]->SetConstantOp(true);
    instrs[i]->SetConstantVal(val);
    instrs[i-1]->SetRedundant(true);
    instrs[i-2]->SetRedundant(true);
  }
}

int LocalOptimizer::EvalConstantOp(const IrInstrPtr& instr, const int val1, const int val2) {
  assert(instr->IsBinOp() == true);

  switch(instr->GetType()) {
  case IrInstrType::ADD_INSTR:
    return val1 + val2;
  case IrInstrType::SUB_INSTR:
    return val1 - val2;
  case IrInstrType::MUL_INSTR:
    return val1 * val2;
  case IrInstrType::DIV_INSTR:
    return val1 / val2;
  case IrInstrType::MOD_INSTR:
    return val1 % val2;
  default:
    return 0;
  }
}

void LocalOptimizer::CheckAndMarkIdentities(const std::vector<IrInstrPtr>& instrs, const int i) {
  if (!instrs[i]->IsBinOp()) {
    return;
  }

  switch (instrs[i]->GetType()) {
  case IrInstrType::ADD_INSTR:
    CheckAndMarkAddIdentity(instrs, i);
    return;
  case IrInstrType::SUB_INSTR:
    CheckAndMarkSubIdentity(instrs, i);
    return;
  case IrInstrType::MUL_INSTR:
    CheckAndMarkMulIdentity(instrs, i);
    return;
  case IrInstrType::DIV_INSTR:
    CheckAndMarkDivIdentity(instrs, i);
    return;
  default:
    return;
  }
}

int LocalOptimizer::GetLvnForFirstArg(const IrInstrPtr& instr) {
  int first_val;
  const std::string first_arg = instr->GetArgs().first;
  const auto val_map_result = val_map_.find(first_arg);

  if (val_map_result == val_map_.end()) {
    first_val = val_count_;
    val_map_.insert(std::pair<std::string, int>(first_arg, first_val));
    val_count_++;
  } else {
    first_val = val_map_result->second;
  }

  return first_val;
}

int LocalOptimizer::GetLvnForSecondArg(const IrInstrPtr& instr, int val1) {
  int second_val;
  const std::string second_arg = instr->GetArgs().second;
  const auto val_map_result = val_map_.find(second_arg);

  if (val_map_result == val_map_.end()) {
    // On a load instruction, we set the local value to be
    // equal to the variable being loaded. Otherwise, we create
    // a new value.
    if (instr->GetType() == IrInstrType::LD_INSTR) {
      second_val = val1;
    } else {
      second_val = val_count_;
      val_count_++;
    }
    val_map_.insert(std::pair<std::string, int>(second_arg, second_val));
  } else {
    second_val = val_map_result->second;
  }

  return second_val;
}

std::string LocalOptimizer::BuildLvnMapKey(const IrInstrPtr& instr, const int val1, const int val2) {
  int min_val;
  int max_val;

  if (instr->IsCommutative()) {
    min_val = std::min(val1, val2);
    max_val = std::max(val1, val2);
  } else {
    min_val = val1;
    max_val = val2;
  }

  std::string key = std::to_string(min_val);
  key += instr->GetTypeAsStr();
  key += std::to_string(max_val);

  return key;
}

void LocalOptimizer::MarkPreviousLdInstrs(const std::vector<IrInstrPtr>& instrs, const int i) {
  if (i >= 1 && instrs[i-1]->GetType() == IrInstrType::LD_INSTR) {
    instrs[i-1]->SetRedundant(true);
  }

  if (i >= 2 && instrs[i-2]->GetType() == IrInstrType::LD_INSTR) {
    instrs[i-2]->SetRedundant(true);
  }
}

void LocalOptimizer::CheckAndMarkAddIdentity(const std::vector<IrInstrPtr>& instrs, const int i) {
  assert(instrs[i]->GetType() == IrInstrType::ADD_INSTR);
  if (i < 2) {
    return;
  }

  const auto first_instr = instrs[i-2];
  const auto second_instr = instrs[i-1];

  auto instr_ord = OrderInstrsByMvThenLd(first_instr, second_instr);
  IrInstrPtr mv_instr = instr_ord.first;
  IrInstrPtr ld_instr = instr_ord.second;

  if (mv_instr->GetArgs().first == "0") {
    // TODO: Possible to remove a following SV instruction too?
    std::string new_dest = instrs[i]->GetDest();
    ld_instr->SetDest(new_dest);
    mv_instr->SetRedundant(true);
    instrs[i]->SetRedundant(true);
  }
}

void LocalOptimizer::CheckAndMarkSubIdentity(const std::vector<IrInstrPtr>& instrs, const int i) {
  assert(instrs[i]->GetType() == IrInstrType::SUB_INSTR);
  if (i < 2) {
    return;
  }

  const auto first_instr = instrs[i-2];
  const auto second_instr = instrs[i-1];
  const auto sub_instr = instrs[i];

  // Optimize a - 0 = a
  OptSubByZero(first_instr, second_instr, sub_instr);

  // Optimize a - a = 0
  OptSubBySelf(first_instr, second_instr, sub_instr);
}

void LocalOptimizer::CheckAndMarkMulIdentity(const std::vector<IrInstrPtr>& instrs, const int i) {
  assert(instrs[i]->GetType() == IrInstrType::MUL_INSTR);
  if (i < 2) {
    return;
  }

  const auto first_instr = instrs[i-2];
  const auto second_instr = instrs[i-1];
  const auto mul_instr = instrs[i];

  // Optimize 2 x a = a + a
  OptMulByTwo(first_instr, second_instr, mul_instr);

  // Optimize a x 1 = a
  OptMulByOne(first_instr, second_instr, mul_instr);

  // Optimize a x 0 = 0
  OptMulByZero(first_instr, second_instr, mul_instr);
}

void LocalOptimizer::CheckAndMarkDivIdentity(const std::vector<IrInstrPtr>& instrs, const int i) {
  // TODO: a / a = 1, as long as a != 0. Do we check the value of a in the symbol table here?
  assert(instrs[i]->GetType() == IrInstrType::DIV_INSTR);
  if (i < 2) {
    return;
  }

  const auto first_instr = instrs[i-2];
  const auto second_instr = instrs[i-1];
  const auto div_instr = instrs[i];

  // Optimize a / 1 = a
  OptDivByOne(first_instr, second_instr, div_instr);
}

void LocalOptimizer::OptSubByZero(const IrInstrPtr& first_instr,
				  const IrInstrPtr& second_instr,
				  const IrInstrPtr& sub_instr) {
  if (IsLdThenMvOrder(first_instr, second_instr) && second_instr->GetArgs().first == "0") {
    // If we have a candidate for optimization, we can replace the first ld
    // instruction's arguments and mark the others for removal.
    std::string new_dest = sub_instr->GetDest();
    std::pair<std::string, std::string> args(first_instr->GetArgs().first, new_dest);
    first_instr->SetDest(new_dest);
    first_instr->SetArgs(args);

    second_instr->SetRedundant(true);
    sub_instr->SetRedundant(true);
  }
}

void LocalOptimizer::OptSubBySelf(const IrInstrPtr& first_instr,
				  const IrInstrPtr& second_instr,
				  const IrInstrPtr& sub_instr) {
  if (IsLdThenLdOrder(first_instr, second_instr) &&
      (first_instr->GetArgs().first == second_instr->GetArgs().first)) {

    // We convert the sub instruction to a mv instruction, and set the value
    // to 0.
    std::string new_dest = sub_instr->GetDest();
    first_instr->SetRedundant(true);
    second_instr->SetRedundant(true);

    sub_instr->SetType(IrInstrType::MV_INSTR);
    std::pair<std::string, std::string> args("0", sub_instr->GetDest());
    sub_instr->SetArgs(args);
  }
}

void LocalOptimizer::OptMulByTwo(const IrInstrPtr& first_instr,
				 const IrInstrPtr& second_instr,
				 const IrInstrPtr& mul_instr) {
  if (IsLdOrMvOrder(first_instr, second_instr)) {
    auto instr_ord = OrderInstrsByMvThenLd(first_instr, second_instr);
    IrInstrPtr mv_instr = instr_ord.first;
    IrInstrPtr ld_instr = instr_ord.second;

    if (mv_instr->GetArgs().first == "2") {
      // Convert the mv to a ld, of the same value as the other ld instr
      mv_instr->SetType(IrInstrType::LD_INSTR);
      std::pair<std::string, std::string> args(ld_instr->GetArgs().first, mv_instr->GetArgs().second);
      mv_instr->SetArgs(args);

      // Convert the mul to an add
      mul_instr->SetType(IrInstrType::ADD_INSTR);
    }
  }
}

void LocalOptimizer::OptMulByOne(const IrInstrPtr& first_instr,
				 const IrInstrPtr& second_instr,
				 const IrInstrPtr& mul_instr) {
  if (IsLdOrMvOrder(first_instr, second_instr)) {
    auto instr_ord = OrderInstrsByMvThenLd(first_instr, second_instr);
    IrInstrPtr mv_instr = instr_ord.first;
    IrInstrPtr ld_instr = instr_ord.second;

    if (mv_instr->GetArgs().first == "1") {
      std::pair<std::string, std::string> args(ld_instr->GetArgs().first, mul_instr->GetDest());
      ld_instr->SetArgs(args);
      ld_instr->SetDest(mul_instr->GetDest());

      mv_instr->SetRedundant(true);
      mul_instr->SetRedundant(true);
    }
  }
}

void LocalOptimizer::OptMulByZero(const IrInstrPtr& first_instr,
				  const IrInstrPtr& second_instr,
				  const IrInstrPtr& mul_instr) {
  if (IsLdOrMvOrder(first_instr, second_instr)) {
    auto instr_ord = OrderInstrsByMvThenLd(first_instr, second_instr);
    IrInstrPtr mv_instr = instr_ord.first;
    IrInstrPtr ld_instr = instr_ord.second;

    if (mv_instr->GetArgs().first == "0") {
      mv_instr->SetDest(mul_instr->GetDest());
      ld_instr->SetRedundant(true);
      mul_instr->SetRedundant(true);
    }
  }
}

void LocalOptimizer::OptDivByOne(const IrInstrPtr& first_instr,
				 const IrInstrPtr& second_instr,
				 const IrInstrPtr& div_instr) {
  if (IsLdThenMvOrder(first_instr, second_instr) && second_instr->GetArgs().first == "1") {
    std::pair<std::string, std::string> args(first_instr->GetArgs().first, div_instr->GetDest());
    first_instr->SetArgs(args);
    first_instr->SetDest(div_instr->GetDest());
    second_instr->SetRedundant(true);
    div_instr->SetRedundant(true);
  }
}

bool LocalOptimizer::IsLdOrMvOrder(const IrInstrPtr& first_instr,
				   const IrInstrPtr& second_instr) {
  auto first_type = first_instr->GetType();
  auto second_type = second_instr->GetType();

  bool mv_first = (first_type == IrInstrType::MV_INSTR && second_type == IrInstrType::LD_INSTR);
  bool ld_first = (first_type == IrInstrType::LD_INSTR && second_type == IrInstrType::MV_INSTR);

  return (mv_first || ld_first);
}

bool LocalOptimizer::IsLdThenMvOrder(const IrInstrPtr& first_instr,
				     const IrInstrPtr& second_instr) {
  auto first_type = first_instr->GetType();
  auto second_type = second_instr->GetType();

  return (first_type == IrInstrType::LD_INSTR && second_type == IrInstrType::MV_INSTR);
}

bool LocalOptimizer::IsLdThenLdOrder(const IrInstrPtr& first_instr,
				     const IrInstrPtr& second_instr) {
  auto first_type = first_instr->GetType();
  auto second_type = second_instr->GetType();

  return (first_type == IrInstrType::LD_INSTR && second_type == IrInstrType::LD_INSTR);
}

std::pair<IrInstrPtr, IrInstrPtr> LocalOptimizer::OrderInstrsByMvThenLd(const IrInstrPtr& first_instr,
									const IrInstrPtr& second_instr) {
  std::pair<IrInstrPtr, IrInstrPtr> instr_pair;

  if (first_instr->GetType() == IrInstrType::MV_INSTR) {
    instr_pair.first = first_instr;
    instr_pair.second = second_instr;
  } else {
    instr_pair.first = second_instr;
    instr_pair.second = first_instr;
  }

  return instr_pair;
}
