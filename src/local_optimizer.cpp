#include <unordered_map>
#include <utility>
#include <iostream>
#include "local_optimizer.h"

LocalOptimizer::LocalOptimizer() {
  val_count_ = 0;
}

void LocalOptimizer::Lvn(CfgNodePtr block) {
  val_count_ = 0;
  val_map_.clear();
  op_map_.clear();

  // We iterate over instrs, but we may also need to alter the instructions
  // vector if we want to remove redundant instructions. So we also keep a copy
  // of the instrs vector to alter at the same time
  // TODO: Maybe better to mark instruction as "erasable" then remove them
  // in a second pass.. If we remove instructions in the middle of the vector
  // the indices might not match up anymore when we want to remove more.
  const auto instrs = block->GetInstrs();
  auto new_instrs = block->GetInstrs();

  for (int i = 0; i < instrs.size() - 1; i++) {
    const auto instr = instrs[i];
    // Skip everything but saves, loads, and binops
    if (!IsInstrLvnValid(instr)) {
      continue;
    }

    int first_val = GetLvnForFirstArg(instr);
    int second_val = GetLvnForSecondArg(instr, first_val);
    std::string key = BuildLvnMapKey(instr, first_val, second_val);
    auto op_map_result = op_map_.find(key);

    if (op_map_result == op_map_.end()) {
      // If the instruction key doesn't exist already, we need to decide if
      // we want to store it. If the value of the operation is never stored in
      // a var, there is nothing to save (because there's nothing to load from
      // later on).
      if (instrs[i+1]->GetType() == IrInstrType::SV_INSTR) {
	std::string dest = instrs[i+1]->GetDest();
	op_map_.insert(std::pair<std::string, std::string>(key, dest));
      }
    } else {
      // If we've found the calculation, we need to check if we plan to store it.
      // To do that, we look at the next instruction and see if it's a SV
      if (instrs[i+1]->GetType() == IrInstrType::SV_INSTR) {
	// We do plan to store it. Now we can remove the original operation,
	// and replace the sv instruction destination with the value from
	// the op_map table
	// TODO: O(n) runtime by calling erase
	// TODO: Eliminate the earlier LD instructions here too
	std::string new_input = op_map_result->second;
	std::string dest = instrs[i+1]->GetDest();

	// TODO: Should we load the stored calc value into a register first?
	new_instrs[i+1]->SetArgs(std::pair<std::string, std::string>(new_input, dest));
	new_instrs.erase(new_instrs.begin() + (i-1));
      }
    }
  }

  block->SetInstrs(new_instrs);
}

bool LocalOptimizer::IsInstrLvnValid(const IrInstrPtr& instr) {
  return (instr->IsBinOp() ||
	  instr->GetType() == IrInstrType::SV_INSTR ||
	  instr->GetType() == IrInstrType::LD_INSTR);
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
  std::string key = std::to_string(val1);
  key += instr->GetTypeAsStr();
  key += std::to_string(val2);

  return key;
}
