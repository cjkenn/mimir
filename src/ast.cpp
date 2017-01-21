#include <iostream>
#include <sstream>
#include "ast.h"

using namespace std;

AstNode::AstNode(AstType type) {
  type_ = type;
  val_ = 0;
}

AstNode::AstNode(AstType type, int val) {
  type_ = type;
  val_ = val;
}

void AstNode::AddChild(AstNodePtr child) {
  children_.push_back(child);
}

AstNodePtr AstNode::GetChildAtIndex(int index) {
  return children_[index];
}

void AstNode::SetDebug() {
  debug_ = "";

  stringstream debug_stream;
  string first_type = GetChildDebugString(children_[0]);
  string second_type = GetChildDebugString(children_[1]);
  string third_type = GetChildDebugString(children_[2]);

  debug_stream << "{\n AstType: " << GetPrettyType() <<
    "\n Text: " << text_ <<
    "\n Value: " << val_ <<
    "\n First Child: " << first_type <<
    "\n Second Child: " << second_type <<
    "\n Third Child: " << third_type <<
    "\n}";

  debug_ = debug_stream.str();
}

void AstNode::Debug() {
  SetDebug();
  cout << debug_ << endl;
}

string AstNode::GetPrettyType() {
  switch(type_) {
  case VAR_AST:
    return "Variable declaration AST";
  case CST_AST:
    return "Constant declaration AST";
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

string AstNode::GetChildDebugString(AstNodePtr child) {
  return (child ? child->GetDebug() : "None");
}
