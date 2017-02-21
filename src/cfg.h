#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "cfg_node.h"

class Cfg {
 public:
  Cfg(CfgNodePtr root);
  ~Cfg() {};

  CfgNodePtr GetCfg() const { return root_; }

  // Calculates and builds the dominance sets for each node in this CFG.
  void Dominance();

 private:
  CfgNodePtr root_;

  // Builds a set of all nodes found in this CFG using a BFS. Order is not
  // important here, but each node will maintain it's set of parents.
  std::vector<CfgNodePtr> BuildCompleteNodeSet();

  // Given a pointer to a node, and a set of nodes in a CFG, this will
  // determine the intersection of all of node's parent's dom sets, and return
  // the intersection.
  //
  // This is intended to be used by the Dominance() function.
  std::vector<CfgNodePtr> FindParentIntersection(CfgNodePtr node, std::vector<CfgNodePtr>& node_set);

  // Given a set of all nodes in thie CFG, builds a simple map from node.name -> NodePtr
  // for easy lookup of each node.
  std::unordered_map<std::string, CfgNodePtr> BuildNodeMapping(std::vector<CfgNodePtr>& cg);
};
