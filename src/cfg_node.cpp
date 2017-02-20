#include <string>
#include <vector>
#include <memory>
#include "cfg_node.h"
#include "ir_instr.h"

CfgNode::CfgNode(std::string name) {
  name_ = name;
  visited_ = false;
}

CfgNode::CfgNode(std::string name, std::vector<CfgNodePtr>& adj) {
  name_ = name;
  visited_ = false;
  adj_ = adj;
}

void CfgNode::AddAdjChild(CfgNodePtr node) {
  adj_.push_back(node);
}

void CfgNode::AddParent(CfgNodePtr node) {
  parents_.push_back(node);
}

void CfgNode::AddInstr(IrInstrPtr instr) {
  instrs_.push_back(instr);
}
