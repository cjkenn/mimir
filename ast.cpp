#include "ast.h"

AstNode::AstNode(AstType type) {
  type_ = type;
  val_ = 0;
  first_child_ = nullptr;
  second_child_ = nullptr;
  third_child_ = nullptr;
}

AstNode::AstNode(AstType type, int val) {
  type_ = type;
  val_ = val;
  first_child_ = nullptr;
  second_child_ = nullptr;
  third_child_ = nullptr;
}

AstNode::~AstNode() {
  first_child_ = nullptr;
  second_child_ = nullptr;
  third_child_ = nullptr;
}

void AstNode::AddChild(AstNode child, int child_index) {
  // TODO: Error handling for anything other than 1-3 index values
  if (child_index == 1) {
    first_child_ = &child;
  }

  if (child_index == 2) {
    second_child_ = &child;
  }

  if (child_index == 3) {
    third_child_ = &child;
  }
}
