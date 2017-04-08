#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "x86_reg.h"

class X86RegAlloc {
 public:
  X86RegAlloc();
  ~X86RegAlloc() {};

  void CheckAndDefineVirtualRegMapping();
  X86RegPtr NextRegister();

  X86RegPtr GetRax();
  X86RegPtr GetRbp();
  X86RegPtr GetRsp();
  int GetVirtualRegCount() const { return virtual_reg_count_; }
  void SetVirtualRegCount(int count) { virtual_reg_count_ = count; }

 private:
  void InitRegisters();
  std::string MapNumOrderToRegName(const int i);
  std::unordered_map<std::string, X86RegPtr> registers_;
  std::vector<X86RegPtr> reg_list_;
  int virtual_reg_count_;
};
