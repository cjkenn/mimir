#include <unordered_map>
#include <utility>
#include "local_optimizer.h"

void LocalOptimizer::Lvn(CfgNodePtr root) {
  int val_count = 0;
  std::unordered_map<std::string, int> val_map;
  std::unordered_map<std::string, std::string> op_map;

  // TODO: Traverse each block here, or call this function for every block?
  for (auto instr : root->GetInstrs()) {
    // Skip everything but saves and binops
    if (!IsInstrLvnValid(instr)) {
      continue;
    }

    // TODO: Refactor these two steps into a helper function
    // TODO: Rename those iterator variables
    std::string first_arg = instr->GetArgs().first;
    int first_val;
    auto look = val_map.find(first_arg);

    if (look == val_map.end()) {
      first_val = val_count;
      val_map.insert(std::pair<std::string, int>(first_arg, first_val));
      val_count++;
    } else {
      first_val = look->second;
    }

    auto second_arg = instr->GetArgs().second;
    int second_val;
    auto look2 = val_map.find(second_arg);

    if (look2 == val_map.end()) {
      // On a load instruction, we set the local value to be
      // equal to the variable being loaded. Otherwise, we create
      // a new value.
      if (instr->GetType() == IrInstrType::LD_INSTR) {
	second_val = first_val;
      } else {
	second_val = val_count;
	val_count++;
      }
      val_map.insert(std::pair<std::string, int>(second_arg, second_val));
    } else {
      second_val = look->second;
    }

    // Now we have both value numbers, so we construct a string to act as the
    // key for the op_map
    std::string key = std::to_string(first_val);
    key += instr->GetTypeAsStr();
    key += std::to_string(second_val);

    // Check the op_map for this key. If it exists, we can replace the
    // instruction(s) with whatever is stored in the table.
    auto look3 = op_map.find(key);

    if (look3 == op_map.end()) {

    } else {
      // We found a redundant instruction, so we replace the next sv
      // instr with the existing one, and remove the binary operation
      // For example:
      // sv b (sub a, d)
      // sub a, d
      // sv d, c
      // becomes
      // sv b, c

    }
  }
}

bool LocalOptimizer::IsInstrLvnValid(IrInstrPtr instr) {
  return (instr->IsBinOp() ||
	  instr->GetType() == IrInstrType::SV_INSTR ||
	  instr->GetType() == IrInstrType::LD_INSTR);
}
