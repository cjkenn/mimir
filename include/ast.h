#pragma once

#include <string>
#include <memory>
#include <vector>

class AstNode;

typedef std::shared_ptr<AstNode> AstNodePtr;

enum AstType {
  VAR_AST,
  CST_AST,
  ADD_AST,
  SUB_AST,
  MUL_AST,
  DIV_AST,
  MOD_AST,
  EQEQ_AST,
  NEQ_AST,
  LT_AST,
  GT_AST,
  LTE_AST,
  GTE_AST,
  SET_AST,
  IF_AST,
  ELSE_AST,
  WHILE_AST,
  SEQ_AST,
  EXPR_AST,
  NEG_AST,
  PROG_AST,
  EMPTY_AST,
  EOF_AST
};

class AstNode {
 public:
  AstNode(AstType type);
  AstNode(AstType type, int val);
  ~AstNode() {};
  void AddChild(AstNodePtr child);
  AstNodePtr GetChildAtIndex(int index);

  AstType GetType() const {return type_;}
  void SetType(AstType type) {type_ = type;}

  std::string GetText() const {return text_;}
  void SetText(std::string text) {text_ = text;}

  int GetVal() const {return val_;}
  void SetVal(int val) {val_ = val;}

  std::vector<AstNodePtr> GetChildren() const {return children_;}
  void SetChildren(std::vector<AstNodePtr> children) {children_ = children;}

  void Visit() { visited_ = true; }
  bool IsVisited() const { return visited_; }
  void SetVisited(bool v) { visited_ = v; }
  void VisitNodeAndChildren();
  bool IsBinOp();
  bool IsCmp();

 private:
  AstType type_;

  // The value is set if the ast needs a number.
  int val_;

  // Text represents the name of the variable an ast can encode.
  std::string text_;

  std::string debug_;
  std::vector<AstNodePtr> children_;
  bool visited_;
  bool needs_ir_label_;
};
