#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "x86_reg.h"
#include "cfg.h"

class X86RegAlloc {
 public:
  X86RegAlloc(const unsigned int virtual_reg_count);
  ~X86RegAlloc() {};

  void Allocate(const CfgNodePtr& block);

 private:
  void InitRegisters();
  X86RegPtr NextRegister(const std::string vreg);
  std::unordered_map<std::string, X86RegPtr> registers_;
  std::unordered_map<std::string, X86RegPtr> v_registers_;

  unsigned int virtual_reg_count_;
  bool enough_regs_;
};
