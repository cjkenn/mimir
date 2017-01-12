#include <string>
#include <vector>
#include <memory>
#include "cfg_node.h"

CFGNode::CFGNode(std::string name) {
  name_ = name;
  visited_ = false;
}

CFGNode::CFGNode(std::string name, std::vector<NodePtr>& adj) {
  name_ = name;
  visited_ = false;
  adj_ = adj;
}

void CFGNode::AddAdjChild(NodePtr node) {
  adj_.push_back(node);
}

void CFGNode::AddParent(NodePtr node) {
  parents_.push_back(node);
}
