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

  void SetDebug();
  std::string GetDebug() const {return debug_;}

  void Debug();

 private:
  AstType type_;
  int val_;
  std::string text_;
  std::string debug_;
  std::vector<AstNodePtr> children_;

  std::string GetPrettyType();
  std::string GetChildDebugString(AstNodePtr child);
};