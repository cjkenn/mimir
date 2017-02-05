#pragma once

#include "cfg.h"
#include "ast.h"

class CfgBuilder {
 public:
  CfgBuilder();
  ~CfgBuilder() {};

 private:
  int block_num_;
  std::string GetNextBlockName();
};
