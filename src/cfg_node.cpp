#include <string>
#include <vector>
#include <memory>
#include "cfg_node.h"

CfgNode::CfgNode(std::string name, CfgNodeType type) {
  name_ = name;
  type_ = type;
  visited_ = false;
}

CfgNode::CfgNode(std::string name, CfgNodeType type, std::vector<CfgNodePtr>& adj) {
  name_ = name;
  type_ = type;
  visited_ = false;
  adj_ = adj;
}

void CfgNode::AddAdjChild(CfgNodePtr node) {
  adj_.push_back(node);
}

void CfgNode::AddParent(CfgNodePtr node) {
  parents_.push_back(node);
}

void CfgNode::AddAstNode(AstNodePtr node) {
  statements_.push_back(node);
}
