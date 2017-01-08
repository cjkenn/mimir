#include <memory>
#include <cstddef>
#include <vector>
#include <queue>
#include "cfg_node.h"
#include "cfg.h"

CFG::CFG(std::shared_ptr<CFGNode> root) {
  root_ = root;
}

void CFG::Dominance() {
  // Initialize vector of all nodes.
  auto node_set = BuildCompleteNodeSet();

  // Set the predecessor sets for each node.
  BuildPredecessorSets(node_set);

  std::vector<std::shared_ptr<CFGNode>> initial;
  initial.push_back(root_);

  root_->SetDom(initial);

  // We use this loop format to skip the root, which has its
  // dom set initially set to itself, rather than the entire set.
  for (std::size_t i = 1; i < complete_graph_.size(); i++) {
    complete_graph_[i]->SetDom(complete_graph_);
  }

  bool changed = true;

  while (changed) {
    changed = false;

    for (auto node : complete_graph_) {
      // TODO: The temp set contains the current item, plus whatever items
      // exist in the immediate predecessor set.
      std::vector<std::shared_ptr<CFGNode>> temp;

      // TODO: test for equality of vectors here
      if (temp != node->GetDom()) {
	node->SetDom(temp);
	changed = true;
      }
    }
  }
}

std::vector<std::shared_ptr<CFGNode>> CFG::BuildCompleteNodeSet() {
  std::queue<std::shared_ptr<CFGNode>> q;
  std::vector<std::shared_ptr<CFGNode>> cg;
  q.push(root_);
  cg.push_back(root_);

  while (!q.empty()) {
    auto node = q.front();
    q.pop();

    for (auto r : node->GetAdj()) {
      if (!r->GetVisited()) {
	r->SetVisited(true);
        cg.push_back(r);
	q.push(r);
      }
    }
  }

  complete_graph_ = cg;
}

void CFG::BuildPredecessorSets(std::vector<std::shared_ptr<CFGNode>>& cg) {
  for (auto node : cg) {
    for (auto child : node->GetAdj()) {
      child->AddPred(node);
    }
  }
}
