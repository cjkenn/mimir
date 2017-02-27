#include <unordered_map>
#include <utility>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include "local_optimizer.h"

LocalOptimizer::LocalOptimizer() {
  val_count_ = 0;
}

void LocalOptimizer::Lvn(CfgNodePtr& block) {
  val_count_ = 0;
  val_map_.clear();
  op_map_.clear();
  const auto instrs = block->GetInstrs();

  for (int i = 0; i < instrs.size() - 1; i++) {
    const auto instr = instrs[i];
    // Skip everything but saves, loads, and binops
    if (!IsInstrLvnValid(instr)) {
      continue;
    }

    // Check if an operation has a constant value. If it does, we can
    // mark the previous MV instructions as redundant, and flag the current op
    // as constant. later, when we alter the instrs vector, we can change
    // constant operations to MVs.
    CheckAndMarkConstantOp(instrs, i);

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

int LocalOptimizer::GetLvnForFirstArg(const IrInstrPtr& instr) {
  int first_val;
  std::string first_arg = instr->GetArgs().first;
  auto val_map_result = val_map_.find(first_arg);

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
  std::string second_arg = instr->GetArgs().second;
  auto val_map_result = val_map_.find(second_arg);

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

std::string LocalOptimizer::BuildLvnMapKey(const IrInstrPtr& instr, int val1, int val2) {
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
