#include <string>
#include <assert.h>
#include "x86_reg.h"

using namespace mimir;

X86Reg::X86Reg(X86RegName name) {
  name_ = name;
  free_ = true;
}

std::string X86Reg::NameToStr() {
  switch(name_) {
  case X86RegName::RAX_REG:
    return "rax";
  case X86RegName::RCX_REG:
    return "rcx";
  case X86RegName::RDX_REG:
    return "rdx";
  case X86RegName::RBX_REG:
    return "rbx";
  case X86RegName::RSP_REG:
    return "rsp";
  case X86RegName::RBP_REG:
    return "rbp";
  case X86RegName::RSI_REG:
    return "rsi";
  case X86RegName::RDI_REG:
    return "rdi";
  case X86RegName::R8_REG:
    return "r8";
  case X86RegName::R9_REG:
    return "r9";
  case X86RegName::R10_REG:
    return "r10";
  case X86RegName::R11_REG:
    return "r11";
  case X86RegName::R12_REG:
    return "r12";
  case X86RegName::R13_REG:
    return "r13";
  case X86RegName::R14_REG:
    return "r14";
  case X86RegName::R15_REG:
    return "r15";
  default:
    assert(false);
  }
}
