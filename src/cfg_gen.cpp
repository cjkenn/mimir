#include <memory>
#include <string>
#include <assert.h>
#include <iostream>
#include "cfg_gen.h"
#include "cfg_node.h"
#include "cfg.h"
#include "ir_instr.h"

CfgGen::CfgGen() {
  block_count_ = -1;
}

Cfg CfgGen::Gen(std::vector<IrInstrPtr> ir) {
  CfgNodePtr root = std::make_shared<CfgNode>(GetNextName());
  std::vector<CfgNodePtr> leader;

  if (ir.empty()) {
    Cfg cfg(root);
    return cfg;
  }

  // First, mark all relevant instructions as leaders.
  // This requires a single pass over the ir. Each leader
  // will be a part of a cfg node, so the number of cfg nodes
  // is equal to the number of leaders.
  ir[0]->SetIsLeader(true);
  root->AddInstr(ir[0]);
  leader.push_back(root);
  CfgNodePtr curr = root;

  for (int i = 1; i < ir.size(); i++) {
    // Any instruction that has a different label than the
    // previous one is a potential jump target, so we
    // mark it as a leader.
    if (ir[i]->GetLabel() != ir[i-1]->GetLabel()) {
      ir[i]->SetIsLeader(true);
      CfgNodePtr new_node = std::make_shared<CfgNode>(GetNextName());
      new_node->AddInstr(ir[i]);
      leader.push_back(new_node);
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
	int to_jmp = FindBlockIndex(instr);
	node->AddAdjChild(leader[to_jmp]);
      }
    }
  }

  // Add dummy entry and exit nodes to the cfg, with special names.
  CfgNodePtr entry = std::make_shared<CfgNode>("entry");
  CfgNodePtr exit = std::make_shared<CfgNode>("exit");
  entry->AddAdjChild(root);
  leader.back()->AddAdjChild(exit);

  Cfg cfg(entry);

  return cfg;
}

std::string CfgGen::GetNextName() {
  block_count_++;
  curr_name_ = "n" + std::to_string(block_count_++);
  return curr_name_;
}

int CfgGen::FindBlockIndex(IrInstrPtr instr) {
  assert(instr->IsJmp() == true);
  assert(instr->GetDest().size() != 0);

  std::string label = instr->GetDest();
  // Label is guaranteed to end with an int. This int should correspond
  // to the count of the leader block, because a leader block is created
  // once a label is found.
  // TODO: Will this change when procedures are implemented?
  char lbl_num = label.back();
  return lbl_num - '0';
}
