#include <assert.h>
#include "x86_reg.h"
#include "x86_reg_alloc.h"

// Constant containing the number of general purpose registers in x86 assembly. We
// define this as any register except rsp, rbp, rsi, or rdi. That is, rax - rdx,
// as well as r8 - r15.
const int NUM_GP_REG = 12;

X86RegAlloc::X86RegAlloc() {
  InitRegisters();
}

void X86RegAlloc::CheckAndDefineVirtualRegMapping() {
  if (virtual_reg_count_ > NUM_GP_REG) {
    return;
  }

  for (int i = 0; i < virtual_reg_count_; i++) {
    std::string name = MapNumOrderToRegName(i);
    reg_list_.push_back(registers_[name]);
  }
}

X86RegPtr X86RegAlloc::NextRegister() {

}

X86RegPtr X86RegAlloc::GetRax() {
  return registers_["rax"];
}

X86RegPtr X86RegAlloc::GetRbp() {
  return registers_["rbp"];
}

X86RegPtr X86RegAlloc::GetRsp() {
  return registers_["rsp"];
}

std::string X86RegAlloc::MapNumOrderToRegName(const int i) {
  switch(i) {
  case 0:
    return "rax";
  case 1:
    return "rbx";
  case 2:
    return "rcx";
  case 3:
    return "rdx";
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
    return "r" + std::to_string(i+4);
  default:
    assert(false);
  }
}

void X86RegAlloc::InitRegisters() {
  X86RegPtr rax = std::make_shared<X86Reg>(X86RegName::RAX_REG);
  registers_["rax"] = rax;

  X86RegPtr rcx = std::make_shared<X86Reg>(X86RegName::RCX_REG);
  registers_["rcx"] = rcx;

  X86RegPtr rdx = std::make_shared<X86Reg>(X86RegName::RDX_REG);
  registers_["rdx"] = rdx;

  X86RegPtr rbx = std::make_shared<X86Reg>(X86RegName::RBX_REG);
  registers_["rbx"] = rbx;

  X86RegPtr rsp = std::make_shared<X86Reg>(X86RegName::RSP_REG);
  registers_["rsp"] = rsp;

  X86RegPtr rbp = std::make_shared<X86Reg>(X86RegName::RBP_REG);
  registers_["rbp"] = rbp;

  X86RegPtr rsi = std::make_shared<X86Reg>(X86RegName::RSI_REG);
  registers_["rsi"] = rsi;

  X86RegPtr rdi = std::make_shared<X86Reg>(X86RegName::RDI_REG);
  registers_["rdi"] = rdi;

  X86RegPtr r8 = std::make_shared<X86Reg>(X86RegName::R8_REG);
  registers_["r8"] = r8;

  X86RegPtr r9 = std::make_shared<X86Reg>(X86RegName::R9_REG);
  registers_["r9"] = r9;

  X86RegPtr r10 = std::make_shared<X86Reg>(X86RegName::R10_REG);
  registers_["r10"] = r10;

  X86RegPtr r11 = std::make_shared<X86Reg>(X86RegName::R11_REG);
  registers_["r11"] = r11;

  X86RegPtr r12 = std::make_shared<X86Reg>(X86RegName::R12_REG);
  registers_["r12"] = r12;

  X86RegPtr r13 = std::make_shared<X86Reg>(X86RegName::R13_REG);
  registers_["r13"] = r13;

  X86RegPtr r14 = std::make_shared<X86Reg>(X86RegName::R14_REG);
  registers_["r14"] = r14;

  X86RegPtr r15 = std::make_shared<X86Reg>(X86RegName::R15_REG);
  registers_["r15"] = r15;
}
