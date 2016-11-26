#include <iostream>
#include <sstream>
#include "ast.h"

using namespace std;

AstNode::AstNode(AstType type) {
  type_ = type;
  val_ = 0;
  first_child_ = nullptr;
  second_child_ = nullptr;
  third_child_ = nullptr;
  SetDebug();
}

AstNode::AstNode(AstType type, int val) {
  type_ = type;
  val_ = val;
  first_child_ = nullptr;
  second_child_ = nullptr;
  third_child_ = nullptr;
  SetDebug();
}

AstNode::~AstNode() {
  first_child_ = nullptr;
  second_child_ = nullptr;
  third_child_ = nullptr;
}

void AstNode::AddChild(AstNode child, int child_index) {
  // TODO: Error handling for anything other than 1-3 index values
  if (child_index > 3) {
    return;
  }

  if (child_index == 1) {
    first_child_ = &child;
  }

  if (child_index == 2) {
    second_child_ = &child;
  }

  if (child_index == 3) {
    third_child_ = &child;
  }

  SetDebug();
}

void AstNode::SetDebug() {
  debug_ = "";

  stringstream debug_stream;
  string first_type = GetChildDebugString(first_child_);
  string second_type = GetChildDebugString(second_child_);
  string third_type = GetChildDebugString(third_child_);

  debug_stream << "{\n AstType: " << GetPrettyType() <<
    "\n Value: " << val_ <<
    "\n First Child: " << first_type <<
    "\n Second Child: " << second_type <<
    "\n Third Child: " << third_type <<
    "\n}";

  debug_ = debug_stream.str();
}

void AstNode::Debug() {
  cout << debug_ << endl;
}

string AstNode::GetPrettyType() {
  switch(type_) {
  case VAR_AST:
    return "Variable declaration AST";
  case CST_AST:
    return "Constant declartation AST";
  case ADD_AST:
    return "Addition expression AST";
  case SUB_AST:
    return "Subtraction expression AST";
  case LT_AST:
    return "Less than expression AST";
  case SET_AST:
    return "Assignment expression AST";
  case IF_AST:
    return "If statement AST";
  case ELSE_AST:
    return "Else statement AST";
  case WHILE_AST:
    return "While statement AST";
  case SEQ_AST:
    return "Sequence AST";
  case EXPR_AST:
    return "Expression AST";
  case PROG_AST:
    return "Top level program AST";
  case EMPTY_AST:
    return "Empty AST node";
  case EOF_AST:
    return "End of file";
  default:
    return "Unknown ast type!";
  }
}

string AstNode::GetChildDebugString(AstNode *child) {
  return (child ? child->GetDebug() : "None");
}
