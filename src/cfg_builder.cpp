#include "ast.h"
#include "cfg_builder.h"
#include "cfg_node.h"

CfgBuilder::CfgBuilder() {
  block_num_ = 0;
}

std::string CfgBuilder::GetNextBlockName() {
  return "b" + block_num_;
}
