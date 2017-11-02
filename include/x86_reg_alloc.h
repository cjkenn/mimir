#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "x86_reg.h"

class X86RegAlloc {
 public:
  X86RegAlloc(const unsigned int virtual_reg_count);
  ~X86RegAlloc() {};

  X86RegPtr NextRegister();
  X86RegPtr NextRegisterForVReg(const std::string vreg);

 private:
  void InitRegisters();
  std::unordered_map<std::string, X86RegPtr> registers_;
  std::unordered_map<std::string, X86RegPtr> v_registers_;

  unsigned int virtual_reg_count_;
  bool enough_regs_;
};
