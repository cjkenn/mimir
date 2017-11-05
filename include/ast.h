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

  AstType GetType() const {return type_;}
  int GetVal() const {return val_;}
  std::string GetText() const {return text_;}
  std::vector<AstNodePtr> GetChildren() const {return children_;}
  bool IsVisited() const { return visited_; }

  void SetType(AstType type) {type_ = type;}
  void SetVal(int val) {val_ = val;}
  void SetText(std::string text) {text_ = text;}
  void SetChildren(std::vector<AstNodePtr> children) {children_ = children;}
  void SetVisited(bool v) { visited_ = v; }

  // Append an ast node to the children vector.
  void AddChild(AstNodePtr child);

  // Returns the child at a specific index. Will error if the index
  // passed in is greater than the size of the children vector.
  AstNodePtr GetChildAtIndex(const unsigned int index);

  // Set the visited flag to true on this node, as well as visit all children
  // and set their visited flags to true as well. This can be used to
  // traverse a branch of the ast and ensure that no other methods will
  // visit the nodes in that branch.
  void VisitNodeAndChildren();

  // True is the ast node is a binary operator.
  bool IsBinOp();

  // True if the ast node is a comparison operator.
  bool IsCmp();

 private:
  AstType type_;

  // The value is set if the ast needs a number.
  int val_;

  // Text represents the name of the variable an ast can encode.
  std::string text_;

  // Contains pointers to each ast node that is a child of this node.
  std::vector<AstNodePtr> children_;

  // Flag indicating whether or not this node has been visited during a search
  // or traversal. If running multiple searches on an ast, this flag should
  // be reset in between.
  bool visited_;
};
