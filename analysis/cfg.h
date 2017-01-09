#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "cfg_node.h"

class CFG {
 public:
  CFG(NodePtr root);
  ~CFG();

  // Calculates and builds the dominance sets for each node in this CFG.
  void Dominance();

 private:
  NodePtr root_;

  // Builds a set of all nodes found in this CFG using a BFS. Order is not
  // important here, but each node will maintain it's set of parents.
  std::vector<NodePtr> BuildCompleteNodeSet();

  // Given a pointer to a node, and a set of nodes in a CFG, this will
  // determine the intersection of all of node's parent's dom sets, and return
  // the intersection.
  //
  // This is intended to be used by the Dominance() function.
  std::vector<NodePtr> FindParentIntersection(NodePtr node, std::vector<NodePtr> node_set);

  // Given a set of all nodes in thie CFG, builds a simple map from node.name -> NodePtr
  // for easy lookup of each node.
  std::unordered_map<std::string, NodePtr> BuildNodeMapping(std::vector<NodePtr> cg);
};
