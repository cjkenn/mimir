#include <memory>
#include <string>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <utility>
#include "cfg_gen.h"
#include "cfg_node.h"
#include "cfg.h"
#include "ir_instr.h"

using namespace mimir;

CfgGen::CfgGen() {
  block_count_ = -1;
}

Cfg CfgGen::Gen(const std::vector<IrInstrPtr>& ir) {
  CfgNodePtr root = std::make_shared<CfgNode>(GetNextName());

  // Leader is a vector because the ordering of the nodes inserted
  // matters, which makes it difficult to use a map instead.
  std::vector<CfgNodePtr> leader;

  // We map the node label (essentially, the leader name) to the index
  // in the leader vector.
  std::unordered_map<std::string, int> leader_node_map;

  if (ir.empty()) {
    Cfg cfg(root);
    return cfg;
  }

  // First, mark all relevant instructions as leaders.
  // This requires a single pass over the ir. Each leader
  // will be a part of a cfg node, so the number of cfg nodes
  // is equal to the number of leaders.
  //
  // Interestingly, this algorithm will not add a leader block of a
  // function definition type IF nothing calls that function. This makes
  // some sense, as control flow will never jmp to that function if nothing
  // calls it. It may be slightly misleading though, because then a defined
  // function shows up in ir but not in the finished assembly.
  ir[0]->SetIsLeader(true);
  root->AddInstr(ir[0]);

  leader.push_back(root);
  leader_node_map.insert(std::make_pair(ir[0]->GetLabel(), leader.size()-1));
  CfgNodePtr curr = root;

  for (unsigned int i = 1; i < ir.size(); i++) {
    // Any instruction that has a different label than the
    // previous one is a potential jump target, so we
    // mark it as a leader.
    if (ir[i]->GetLabel() != ir[i-1]->GetLabel()) {
      ir[i]->SetIsLeader(true);

      CfgNodePtr new_node = std::make_shared<CfgNode>(GetNextName());
      new_node->AddInstr(ir[i]);
      leader.push_back(new_node);
      leader_node_map.insert(std::make_pair(ir[i]->GetLabel(), leader.size()-1));

      curr = new_node;
    } else {
      curr->AddInstr(ir[i]);
    }
  }

  // Iterate over the leader array and each instruction in the
  // node in the leader array. For any instruction that is a jump
  // we create an edge from the current node to the node that contains
  // the destination of the jump.
  // This runs in O(n), where n is the number of instructions in the ir.
  for (auto node : leader) {
    for (auto instr : node->GetInstrs()) {
      if (instr->IsJmp()) {
	auto jmp_idx = leader_node_map.find(instr->GetDest());
	if (jmp_idx != leader_node_map.end()) {
	  node->AddAdjChild(leader[jmp_idx->second]);
	}
      }

      // TODO: This should be done for a call to a function, not the definition
      // of a function. It doesn't matter really what block the definition goes in,
      // but we need to make sure we jump to the right one when we call it (ie.
      // we get the control flow correct). We also then need to find all ret
      // instructions, and make sure they map back to the calling block
      // if (instr->IsFunc()) {
      // 	auto func_idx = leader_node_map.find(instr->GetLabel());
      // 	if (func_idx != leader_node_map.end()) {
      // 	  node->AddAdjChild(leader[func_idx->second]);
      // 	}
      // }
    }
  }

  // Add dummy entry and exit nodes to the cfg, with special names.
  CfgNodePtr entry = std::make_shared<CfgNode>("entry");
  CfgNodePtr exit = std::make_shared<CfgNode>("exit");
  entry->AddAdjChild(root);
  leader.back()->AddAdjChild(exit);

  Cfg cfg(root);

  return cfg;
}

std::string CfgGen::GetNextName() {
  block_count_++;
  curr_name_ = "n" + std::to_string(block_count_++);
  return curr_name_;
}
