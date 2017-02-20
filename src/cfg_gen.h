#pragma once

#include "cfg.h"
#include "ir_instr.h"

class CfgGen {
 public:
  CfgGen();
  ~CfgGen() {};

  Cfg Gen(std::vector<IrInstrPtr> ir);

 private:
  std::string GetNextName();
  int FindBlockIndex(IrInstrPtr instr);

  std::string curr_name_;
  int block_count_;
};
