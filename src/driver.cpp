#include <memory>
#include <iostream>
#include "cfg.h"
#include "cfg_node.h"

int main(int argc, char** argv) {
  auto b0 = std::make_shared<CfgNode>("b0", CfgNodeType::BASIC_CFG);
  auto b1 = std::make_shared<CfgNode>("b1", CfgNodeType::BASIC_CFG);
  auto b2 = std::make_shared<CfgNode>("b2", CfgNodeType::BASIC_CFG);

  std::vector<CfgNodePtr> b0_parents;
  b0->SetParents(b0_parents);
  b0->AddAdjChild(b1);

  std::vector<CfgNodePtr> b1_parents;
  b1_parents.push_back(b0);
  b1->SetParents(b1_parents);
  b1->AddAdjChild(b2);

  std::vector<CfgNodePtr> b2_parents;
  b2_parents.push_back(b1);
  b2->SetParents(b2_parents);

  Cfg cfg(b0);

  cfg.Dominance();

  auto dom = b2->GetDom();

  for (auto &node : dom) {
    std::cout << node->GetName() << std::endl;
  }

  return 0;
}
