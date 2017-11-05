#include <assert.h>
#include <queue>
#include <iostream>
#include "x86_reg.h"
#include "x86_reg_alloc.h"
#include "cfg.h"
#include "x86_instr_arg.h"

// Constant containing the number of general purpose registers in x86 assembly. We
// define this as any register except rsp, rbp, rsi, or rdi. That is, rax - rdx,
// as well as r8 - r15.
// TODO: However, the amount of mul, div, and mod instructions should (might?) affect
// this because they require certain registers. Might need to remove the x86 registers
// from those intrs selections in favor of virtual ones and then allocate later, in this phase
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
    // instrs is a vector of x86 instrs in the order they appear
    auto instrs = block->GetX86Instrs();
    for (auto instr : instrs) {
      auto first_arg = instr->GetFirstArg();

      if (first_arg) {
	if (first_arg->GetType() == X86InstrArgType::REG_X86) {
	  auto reg1 = NextRegister(first_arg->GetVal());
	  first_arg->SetVal(reg1->NameToStr());
	}
      }

      auto second_arg = instr->GetSecondArg();

      if (second_arg) {
	if (second_arg->GetType() == X86InstrArgType::REG_X86) {
	  auto reg2 = NextRegister(second_arg->GetVal());
	  second_arg->SetVal(reg2->NameToStr());
	}
      }
    }

    for (auto b : block->GetAdj()) {
      if (!b->GetAllocVisited()) {
	q.push(b);
      }
    }
  }
}

X86RegPtr X86RegAlloc::NextRegister(const std::string vreg) {
  if (enough_regs_) {
    auto reg = v_registers_[vreg];
    reg->Allocate();
    return reg;
  }

  // TODO: segfaults will happen here
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
