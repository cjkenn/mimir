#include <string>
#include <vector>
#include <memory>
#include "cfg_node.h"

CFGNode::CFGNode(std::string name) {
  name_ = name;
  visited_ = false;
}

CFGNode::CFGNode(std::string name, std::vector<std::shared_ptr<CFGNode>> adj) {
  name_ = name;
  visited_ = false;
  adj_ = adj;
}

void CFGNode::AddAdjChild(std::shared_ptr<CFGNode> node) {
  adj_.push_back(node);
}

void CFGNode::AddPred(std::shared_ptr<CFGNode> node) {
  preds_.push_back(node);
}
