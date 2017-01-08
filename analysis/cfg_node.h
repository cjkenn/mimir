#pragma once

#include <string>
#include <vector>
#include <memory>

class CFGNode {
 public:
  CFGNode(std::string name);
  CFGNode(std::string name, std::vector<std::shared_ptr<CFGNode>> adj);
  ~CFGNode();

  std::string GetName() const { return name_; }
  bool GetVisited() const { return visited_; }
  std::vector<std::shared_ptr<CFGNode>> GetDom() const { return dom_; }
  std::vector<std::shared_ptr<CFGNode>> GetAdj() const { return adj_; }
  std::vector<std::shared_ptr<CFGNode>> GetPreds() const { return preds_; }

  bool SetVisited(bool visited) { visited_ = visited; }
  void SetDom(std::vector<std::shared_ptr<CFGNode>> dom) { dom_ = dom; }
  void SetAdj(std::vector<std::shared_ptr<CFGNode>> adj) { adj_ = adj; }
  void SetPreds(std::vector<std::shared_ptr<CFGNode>> preds) { preds_ = preds; }

  void AddAdjChild(std::shared_ptr<CFGNode> node);
  void AddPred(std::shared_ptr<CFGNode> node);

 private:
  std::string name_;
  bool visited_;
  std::vector<std::shared_ptr<CFGNode>> dom_;
  std::vector<std::shared_ptr<CFGNode>> adj_;
  std::vector<std::shared_ptr<CFGNode>> preds_;
};
