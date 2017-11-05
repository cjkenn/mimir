#include <queue>
#include <assert.h>
#include "ast.h"

AstNode::AstNode(AstType type) {
  type_ = type;
  val_ = 0;
  visited_ = false;
}

AstNode::AstNode(AstType type, int val) {
  type_ = type;
  val_ = val;
  visited_ = false;
}

void AstNode::AddChild(AstNodePtr child) {
  children_.push_back(child);
}

AstNodePtr AstNode::GetChildAtIndex(const unsigned int index) {
  assert(index < children_.size());
  return children_[index];
}

void AstNode::VisitNodeAndChildren() {
  visited_ = true;
  for (auto node : children_) {
    node->SetVisited(true);
  }
}

bool AstNode::IsBinOp() {
  return (type_ == AstType::ADD_AST ||
	  type_ == AstType::SUB_AST ||
	  type_ == AstType::DIV_AST ||
	  type_ == AstType::MUL_AST ||
	  type_ == AstType::MOD_AST);
}

bool AstNode::IsCmp() {
  return (type_ == AstType::EQEQ_AST ||
	  type_ == AstType::NEQ_AST ||
	  type_ == AstType::LT_AST ||
	  type_ == AstType::GT_AST ||
	  type_ == AstType::LTE_AST ||
	  type_ == AstType::GTE_AST);
}
