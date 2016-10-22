#pragma once
#include <string>

enum AstType {
  VAR_AST,
  CST_AST,
  ADD_AST,
  SUB_AST,
  LT_AST,
  SET_AST,
  IF_AST,
  ELSE_AST,
  WHILE_AST,
  SEQ_AST,
  EXPR_AST,
  PROG_AST,
  EMPTY_AST,
  EOF_AST
};

class AstNode {
 public:
  AstNode(AstType type);
  AstNode(AstType type, int val);
  ~AstNode();
  void AddChild(AstNode child, int child_index);

  AstType GetType() const {return type_;}
  void SetType(AstType type) {type_ = type;}

  std::string GetText() const {return text_;}
  void SetText(std::string text) {text_ = text;}

  int GetVal() const {return val_;}
  void SetVal(int val) {val_ = val;}

 private:
  AstType type_;
  int val_;
  std::string text_;
  AstNode *first_child_;
  AstNode *second_child_;
  AstNode *third_child_;
};
