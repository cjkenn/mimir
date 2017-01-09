#pragma once

#include <string>
#include <vector>
#include <memory>

class CFGNode;

typedef std::shared_ptr<CFGNode> NodePtr;

class CFGNode {
 public:
  CFGNode(std::string name);
  CFGNode(std::string name, std::vector<NodePtr> adj);
  ~CFGNode() {};

  std::string GetName() const { return name_; }
  bool GetVisited() const { return visited_; }
  std::vector<NodePtr> GetDom() const { return dom_; }
  std::vector<NodePtr> GetAdj() const { return adj_; }
  std::vector<NodePtr> GetParents() const { return parents_; }

  void SetVisited(bool visited) { visited_ = visited; }
  void SetDom(std::vector<NodePtr> dom) { dom_ = dom; }
  void SetAdj(std::vector<NodePtr> adj) { adj_ = adj; }
  void SetParents(std::vector<NodePtr> parents) { parents_ = parents; }

  void AddAdjChild(NodePtr node);
  void AddParent(NodePtr node);

 private:
  std::string name_;
  bool visited_;
  std::vector<NodePtr> dom_;
  std::vector<NodePtr> adj_;
  std::vector<NodePtr> parents_;
};
