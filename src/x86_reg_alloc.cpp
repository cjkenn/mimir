#include <assert.h>
#include <queue>
#include "x86_reg.h"
#include "x86_reg_alloc.h"
#include "cfg.h"

// Constant containing the number of general purpose registers in x86 assembly. We
// define this as any register except rsp, rbp, rsi, or rdi. That is, rax - rdx,
// as well as r8 - r15.
const int NUM_GP_REG = 12;

X86RegAlloc::X86RegAlloc(const unsigned int virtual_reg_count) {
  InitRegisters();
  virtual_reg_count_ = virtual_reg_count;

  enough_regs_ = (virtual_reg_count < NUM_GP_REG ? true : false);
}

void X86RegAlloc::Allocate(const CfgNodePtr& block) {
  std::queue<CfgNodePtr> q;
  q.push(block);

  while(!q.empty()) {
    auto block = q.front();
    q.pop();
    block->SetAllocVisited(true);
    // Allocate block here


    for (auto b : block->GetAdj()) {
      if (!b->GetAllocVisited()) {
	q.push(b);
      }
    }
  }
}

X86RegPtr X86RegAlloc::NextRegister() {

}

X86RegPtr X86RegAlloc::NextRegisterForVReg(const std::string vreg) {
  if (enough_regs_) {
    auto reg = v_registers_[vreg];
    reg->Allocate();
    return reg;
  }
}

void X86RegAlloc::InitRegisters() {
  X86RegPtr rax = std::make_shared<X86Reg>(X86RegName::RAX_REG);
  registers_["rax"] = rax;
  v_registers_["r0"] = rax;

  X86RegPtr rbx = std::make_shared<X86Reg>(X86RegName::RBX_REG);
  registers_["rbx"] = rbx;
  v_registers_["r1"] = rbx;

  X86RegPtr rcx = std::make_shared<X86Reg>(X86RegName::RCX_REG);
  registers_["rcx"] = rcx;
  v_registers_["r2"] = rcx;

  X86RegPtr rdx = std::make_shared<X86Reg>(X86RegName::RDX_REG);
  registers_["rdx"] = rdx;
  v_registers_["r3"] = rdx;

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
  v_registers_["r4"] = r8;

  X86RegPtr r9 = std::make_shared<X86Reg>(X86RegName::R9_REG);
  registers_["r9"] = r9;
  v_registers_["r5"] = r9;

  X86RegPtr r10 = std::make_shared<X86Reg>(X86RegName::R10_REG);
  registers_["r10"] = r10;
  v_registers_["r6"] = r10;

  X86RegPtr r11 = std::make_shared<X86Reg>(X86RegName::R11_REG);
  registers_["r11"] = r11;
  v_registers_["r7"] = r11;

  X86RegPtr r12 = std::make_shared<X86Reg>(X86RegName::R12_REG);
  registers_["r12"] = r12;
  v_registers_["r8"] = r12;

  X86RegPtr r13 = std::make_shared<X86Reg>(X86RegName::R13_REG);
  registers_["r13"] = r13;
  v_registers_["r9"] = r13;

  X86RegPtr r14 = std::make_shared<X86Reg>(X86RegName::R14_REG);
  registers_["r14"] = r14;
  v_registers_["r10"] = r14;

  X86RegPtr r15 = std::make_shared<X86Reg>(X86RegName::R15_REG);
  registers_["r15"] = r15;
  v_registers_["r11"] = r15;
}
