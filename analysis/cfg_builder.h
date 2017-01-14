#pragma once

#include "cfg.h"
#include "../ast.h"

class CfgBuilder {
 public:
  CfgBuilder();
  ~CfgBuilder() {};
  // Builds a complete CFG given an Ast from a parser.
  // O(n), where n is the number of Ast nodes in the tree.
  CfgNodePtr BuildCfgFromAst(AstNodePtr root);

 private:
  int block_num_;
  CfgNodeType GetCfgTypeFromAst(AstNodePtr node);
  CfgNodePtr MakeCfgNode(AstNodePtr node);
  std::string GetNextBlockName();
};
