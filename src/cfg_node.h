#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ir_instr.h"

class CfgNode;

typedef std::shared_ptr<CfgNode> CfgNodePtr;

class CfgNode {
 public:
  CfgNode(std::string name);
  CfgNode(std::string name, std::vector<CfgNodePtr>& adj);
  ~CfgNode() {};

  std::string GetName() const { return name_; }
  bool GetVisited() const { return visited_; }

  std::vector<CfgNodePtr> GetDom() const { return dom_; }
  std::vector<CfgNodePtr> GetAdj() const { return adj_; }
  std::vector<CfgNodePtr> GetParents() const { return parents_; }
  std::vector<IrInstrPtr> GetInstrs() const { return instrs_; }

  void SetVisited(bool visited) { visited_ = visited; }
  void SetDom(std::vector<CfgNodePtr> dom) { dom_ = dom; }
  void SetAdj(std::vector<CfgNodePtr> adj) { adj_ = adj; }
  void SetParents(std::vector<CfgNodePtr> parents) { parents_ = parents; }
  void SetInstrs(std::vector<IrInstrPtr> instrs) { instrs_ = instrs; }

  void AddAdjChild(CfgNodePtr node);
  void AddParent(CfgNodePtr node);
  void AddInstr(IrInstrPtr instr);

 private:
  // The unique name of this node.
  std::string name_;

  // If this node has been visited. Should be reset before
  // traversals, just in case.
  bool visited_;

  // Dominance set.
  std::vector<CfgNodePtr> dom_;

  // Adjacent nodes set.
  std::vector<CfgNodePtr> adj_;

  // Immediate predecessors set.
  std::vector<CfgNodePtr> parents_;

  // Contains each ast node in the basic block.
  std::vector<IrInstrPtr> instrs_;
};
