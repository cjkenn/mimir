#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ast.h"

enum CfgNodeType {
  IF_CFG = 0,
  WHILE_CFG = 1,
  BASIC_CFG = 2
};

class CfgNode;

typedef std::shared_ptr<CfgNode> CfgNodePtr;

class CfgNode {
 public:
  CfgNode(std::string name, CfgNodeType type);
  CfgNode(std::string name, CfgNodeType type, std::vector<CfgNodePtr>& adj);
  ~CfgNode() {};

  std::string GetName() const { return name_; }
  bool GetVisited() const { return visited_; }
  CfgNodeType GetType() const { return type_; }
  std::vector<CfgNodePtr> GetDom() const { return dom_; }
  std::vector<CfgNodePtr> GetAdj() const { return adj_; }
  std::vector<CfgNodePtr> GetParents() const { return parents_; }
  std::vector<AstNodePtr> GetStatements() const { return statements_; }

  void SetVisited(bool visited) { visited_ = visited; }
  void SetDom(std::vector<CfgNodePtr> dom) { dom_ = dom; }
  void SetAdj(std::vector<CfgNodePtr> adj) { adj_ = adj; }
  void SetParents(std::vector<CfgNodePtr> parents) { parents_ = parents; }
  void SetStatements(std::vector<AstNodePtr> statements) { statements_ = statements; }

  void AddAdjChild(CfgNodePtr node);
  void AddParent(CfgNodePtr node);
  void AddAstNode(AstNodePtr node);

 private:
  // The unique name of this node.
  std::string name_;

  // If this node has been visited. Should be reset before
  // traversals, just in case.
  bool visited_;

  // The enum type of this node.
  CfgNodeType type_;

  // Dominance set.
  std::vector<CfgNodePtr> dom_;

  // Adjacent nodes set.
  std::vector<CfgNodePtr> adj_;

  // Immediate predecessors set.
  std::vector<CfgNodePtr> parents_;

  // Contains each ast node in the basic block.
  std::vector<AstNodePtr> statements_;
};
