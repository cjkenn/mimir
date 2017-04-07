#pragma once

#include <string>
#include <memory>

class X86Reg;

typedef std::shared_ptr<X86Reg> X86RegPtr;

enum X86RegName {
  RAX_REG,
  RCX_REG,
  RDX_REG,
  RBX_REG,
  RSP_REG,
  RBP_REG,
  RSI_REG,
  RDI_REG,
  R8_REG,
  R9_REG,
  R10_REG,
  R11_REG,
  R12_REG,
  R13_REG,
  R14_REG,
  R15_REG
};

class X86Reg {
 public:
  X86Reg(X86RegName name);
  ~X86Reg() {};

  X86RegName GetName() const { return name_; }
  bool IsFree() const { return free_; }
  std::string NameToStr();

 private:
  X86RegName name_;
  bool free_;
};
