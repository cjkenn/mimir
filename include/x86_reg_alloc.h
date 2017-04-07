#pragma once

#include <string>
#include <unordered_map>
#include "x86_reg.h"

class X86RegAlloc {
 public:
  X86RegAlloc();
  ~X86RegAlloc() {};

  X86RegPtr GetRax();
  X86RegPtr GetRbp();
  X86RegPtr GetRsp();

 private:
  void InitRegisters();
  std::unordered_map<std::string, X86RegPtr> registers_;
};
