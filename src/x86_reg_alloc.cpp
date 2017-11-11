#include <assert.h>
#include <queue>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
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
const unsigned int NUM_GP_REG = 12;

// r15 is reserved as a spill register to ld/sv values that we cannot map to a real register,
// so our unreserved register count is one less than the number of general purpose registers.
const unsigned int NUM_UNRESERVED_REG = 11;

X86RegAlloc::X86RegAlloc(const unsigned int virtual_reg_count) {
  InitRegisters();
  virtual_reg_count_ = virtual_reg_count;
}

void X86RegAlloc::Allocate(const CfgNodePtr& block) {
  std::queue<CfgNodePtr> q;
  q.push(block);

  while(!q.empty()) {
    auto block = q.front();
    q.pop();
    block->SetAllocVisited(true);
    auto instrs = block->GetX86Instrs();

    // In a linear pass, count the number of times each register
    // appears. Use this count as the virtual register's priority.
    for (auto instr : instrs) {
      CountReg(instr);
    }

    // Assign registers to the 12 highest priority virtual regs.
    // Use the registers map to access created registers, and
    // create an assigned mapping.
    std::vector<std::pair<std::string, unsigned int>> sorted_vregs;
    for (auto i = reg_cnts_.begin(); i != reg_cnts_.end(); i++) {
      sorted_vregs.push_back(*i);
    }

    std::sort(sorted_vregs.begin(), sorted_vregs.end(),
	      [=](std::pair<std::string, unsigned int>& a, std::pair<std::string, unsigned int>& b) {
		return b.second < a.second;
	      });

    // Now pick the top NUM_UNRESERVED_REG and assign to a real register. Any virtual registers
    // left unassigned will be spilled.
    // assigned_regs maps the virtual register name to a real register name (ie. r0 -> rax)
    std::unordered_map<std::string, X86RegPtr> assigned_regs;
    const unsigned int len = std::min((unsigned int)sorted_vregs.size(), NUM_UNRESERVED_REG);

    for (unsigned int i = 0; i < len; i++) {
      assigned_registers_[sorted_vregs[i].first] = registers_[reg_order_[i]];
    }

    // TODO: Traverse the instructions again, and rewrite the instructions
    // to use the assigned register. If no register is assigned for the
    // found virtual reg, insert a ld from memory and a sv to a reserved
    // register to access the value.
    // TODO: Ensure that the stack size is calculated correctly for this operation,
    // as each spilled value adds to the stack size, and the address must be know
    // for writing to be correct.
    // At this time, reg alloc only works when the program needs less than 11 general
    // purpose registers.
    // Steps that may need to be taken: count the amount of extra virtual registers. Determine
    // extra stack space needed from these and then ensure that the writer accomodates that.
    // Each time a vreg is not found in the assigned_regs mapping, change that set of instructions
    // to a mv to the stack, and then a ld from the stack into r15, our reserved register.
    // ie.
    // mov r20, 10
    // becomes
    // mov [rbp-8], 10
    // mov r15, [rbp-8]
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
  // TODO: fix this for when the vreg doesn't exist in the assigned map
  // (ie. not enough registers)
  assert(assigned_registers_.find(vreg) != assigned_registers_.end());

  auto reg = assigned_registers_[vreg];
  reg->Allocate();

  return reg;

}

void X86RegAlloc::CountReg(const X86InstrPtr instr) {
  auto first_arg = instr->GetFirstArg();
  if (first_arg && first_arg->GetType() == X86InstrArgType::REG_X86) {
    reg_cnts_[first_arg->GetVal()]++;
  }

  auto second_arg = instr->GetSecondArg();
  if (second_arg && second_arg->GetType() == X86InstrArgType::REG_X86) {
    reg_cnts_[second_arg->GetVal()]++;
  }
}

void X86RegAlloc::InitRegisters() {
  reg_order_.push_back("rax");
  reg_order_.push_back("rbx");
  reg_order_.push_back("rcx");
  reg_order_.push_back("rdx");
  reg_order_.push_back("r8");
  reg_order_.push_back("r9");
  reg_order_.push_back("r10");
  reg_order_.push_back("r11");
  reg_order_.push_back("r12");
  reg_order_.push_back("r13");
  reg_order_.push_back("r14");
  reg_order_.push_back("r15");

  X86RegPtr rax = std::make_shared<X86Reg>(X86RegName::RAX_REG);
  registers_["rax"] = rax;

  X86RegPtr rbx = std::make_shared<X86Reg>(X86RegName::RBX_REG);
  registers_["rbx"] = rbx;

  X86RegPtr rcx = std::make_shared<X86Reg>(X86RegName::RCX_REG);
  registers_["rcx"] = rcx;

  X86RegPtr rdx = std::make_shared<X86Reg>(X86RegName::RDX_REG);
  registers_["rdx"] = rdx;

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
