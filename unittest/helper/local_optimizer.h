#pragma once

#include "cfg_node.h"
#include "ir_instr.h"

class LocalOptimizerTestHelper {
 public:
  LocalOptimizerTestHelper() {};
  ~LocalOptimizerTestHelper() {};

  CfgNodePtr GetRedundantBlock();
  CfgNodePtr GetCommutativeBlock();
  CfgNodePtr GetNonCommutativeBlock();
  CfgNodePtr GetConstFoldBlock();
  CfgNodePtr GetAddIdBlock();
  CfgNodePtr GetSubByZeroBlock();
  CfgNodePtr GetSubBySelfBlock();
  CfgNodePtr GetMulByTwoBlock();
  CfgNodePtr GetMulByOneBlock();
  CfgNodePtr GetMulByZeroBlock();
  CfgNodePtr GetDivByOneBlock();
};
