#include <memory>
#include <cstddef>
#include <vector>
#include <queue>
#include <unordered_map>
#include "cfg_node.h"
#include "cfg.h"

CFG::CFG(NodePtr root) {
  root_ = root;
}

void CFG::Dominance() {
  if (root_ == nullptr) {
    return;
  }

  // Initialize vector of all nodes.
  auto node_set = BuildCompleteNodeSet();

  std::vector<NodePtr> initial;
  initial.push_back(root_);

  root_->SetDom(initial);

  // We use this loop format to skip the root, which has its
  // dom set initially set to itself, rather than the entire set.
  for (std::size_t i = 1; i < node_set.size(); i++) {
    node_set[i]->SetDom(node_set);
  }

  bool changed = true;

  while (changed) {
    changed = false;

    for (std::size_t i = 1; i < node_set.size(); i++) {
      // The new_dom set contains the current item, plus whatever items
      // exist in the dom sets of all the parent nodes (the intersection of those dom sets)
      auto new_dom = FindParentIntersection(node_set[i], node_set);
      new_dom.push_back(node_set[i]);

      // If the new_dom set is not the same as what's currently set for this node,
      // replace it and continue changing the dom sets.
      if (new_dom != node_set[i]->GetDom()) {
	node_set[i]->SetDom(new_dom);
	changed = true;
      }
    }
  }
}

std::vector<NodePtr> CFG::BuildCompleteNodeSet() {
  std::queue<NodePtr> q;
  std::vector<NodePtr> cg;

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

  return cg;
}

std::vector<NodePtr> CFG::FindParentIntersection(NodePtr node, std::vector<NodePtr> node_set) {
  auto parents = node->GetParents();
  int num_parents = parents.size();

  if (num_parents == 0) {
    return parents;
  }

  if (num_parents == 1) {
    return parents[0]->GetDom();
  }

  std::unordered_map<std::string, int> node_map;
  // 1. Add each node in the first parent's dom set to a map as a key.
  // The value in the map will indicate the counts.
  for (auto node : parents[0]->GetDom()) {
    node_map[node->GetName()] = 1;
  }

  // 2. Loop through the rest of the parent's dom sets, checking if
  // the node found is in the map. If it is, we increment the value.
  for (int i = 1; i < num_parents; i++) {
    for (auto node : parents[i]->GetDom()) {
      auto dom_node = node_map.find(node->GetName());

      if (dom_node != node_map.end()) {
	node_map[node->GetName()] = dom_node->second + 1;
      }
    }
  }

  // 3. Loop though the map, and for any values that are equal to the
  // size of the parents vector, we add to the intersection set. To simplify,
  // we lookup the actual node pointer by using the name. This way we
  // don't need to try and compare NodePtr's with one another.
  std::vector<NodePtr> intersection_set;
  auto all_nodes = BuildNodeMapping(node_set);

  for (auto &entry : node_map) {
    if (entry.second == num_parents) {

      intersection_set.push_back(all_nodes[entry.first]);
    }
  }

  return intersection_set;
}

std::unordered_map<std::string, NodePtr> CFG::BuildNodeMapping(std::vector<NodePtr> cg) {
  std::unordered_map<std::string, NodePtr> node_map;

  for (auto node : cg) {
    node_map[node->GetName()] = node;
  }

  return node_map;
}
