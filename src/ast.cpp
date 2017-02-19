#include <iostream>
#include <sstream>
#include <queue>
#include "ast.h"

AstNode::AstNode(AstType type) {
  type_ = type;
  val_ = 0;
  visited_ = false;
  needs_ir_label_ = false;
}

AstNode::AstNode(AstType type, int val) {
  type_ = type;
  val_ = val;
  visited_ = false;
  needs_ir_label_ = false;
}

void AstNode::AddChild(AstNodePtr child) {
  children_.push_back(child);
}

AstNodePtr AstNode::GetChildAtIndex(int index) {
  return children_[index];
}

void AstNode::VisitNodeAndChildren() {
  visited_ = true;
  for (auto node : children_) {
    node->Visit();
  }
}
