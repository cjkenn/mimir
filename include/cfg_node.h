#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ir_instr.h"
#include "x86_instr.h"

class CfgNode;

typedef std::shared_ptr<CfgNode> CfgNodePtr;

class CfgNode {
 public:
  CfgNode(std::string name);
  CfgNode(std::string name, std::vector<CfgNodePtr>& adj);
  ~CfgNode() {};

  std::string GetName() const { return name_; }
  bool GetVisited() const { return visited_; }
  bool GetSelectorVisited() const { return selector_visited_; }
  bool GetWriterVisited() const { return writer_visited_; }
  bool GetAllocVisited() const { return alloc_visited_; }
  bool GetOptimizerVisited() const { return optimizer_visited_; }

  std::vector<CfgNodePtr> GetDom() const { return dom_; }
  std::vector<CfgNodePtr> GetAdj() const { return adj_; }
  std::vector<CfgNodePtr> GetParents() const { return parents_; }
  std::vector<IrInstrPtr> GetInstrs() const { return instrs_; }
  std::vector<X86InstrPtr> GetX86Instrs() const { return x86instrs_; }

  void SetSelectorVisited(bool visited) { selector_visited_ = visited; }
  void SetWriterVisited(bool visited) { writer_visited_ = visited; }
  void SetAllocVisited(bool visited) { alloc_visited_ = visited; }
  void SetOptimizerVisited(bool visited) { optimizer_visited_ = visited; }
  void SetVisited(bool visited) { visited_ = visited; }

  void SetDom(std::vector<CfgNodePtr> dom) { dom_ = dom; }
  void SetAdj(std::vector<CfgNodePtr> adj) { adj_ = adj; }
  void SetParents(std::vector<CfgNodePtr> parents) { parents_ = parents; }
  void SetInstrs(std::vector<IrInstrPtr> instrs) { instrs_ = instrs; }
  void SetX86Instrs(std::vector<X86InstrPtr> x86instrs) { x86instrs_ = x86instrs; }

  void AddAdjChild(CfgNodePtr node);
  void AddParent(CfgNodePtr node);
  void AddInstr(IrInstrPtr instr);

 private:
  // The unique name of this node.
  std::string name_;

  // If this node has been visited.
  bool visited_;

  // If the instruction selector has visited this node.
  bool selector_visited_;

  // If the instruction writer has visited this node.
  bool writer_visited_;

  // If the register allocator has visited this node.
  bool alloc_visited_;

  // If the local optimizer has visited this node.
  bool optimizer_visited_;

  // Dominance set.
  std::vector<CfgNodePtr> dom_;

  // Adjacent nodes set.
  std::vector<CfgNodePtr> adj_;

  // Immediate predecessors set.
  std::vector<CfgNodePtr> parents_;

  // Contains each ir instruction in this block.
  std::vector<IrInstrPtr> instrs_;

  // Contains each x86 instruction in this block.
  std::vector<X86InstrPtr> x86instrs_;
};
