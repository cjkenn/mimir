#pragma once

#include <memory>
#include "cfg_node.h"

class CFG {
 public:
  CFG(std::shared_ptr<CFGNode> root);
  ~CFG();

  void Dominance();

 private:
  std::shared_ptr<CFGNode> root_;
  std::vector<std::shared_ptr<CFGNode>> complete_graph_;

  std::vector<std::shared_ptr<CFGNode>> BuildCompleteNodeSet();
  void BuildPredecessorSets(std::vector<std::shared_ptr<CFGNode>>& cg);
};
