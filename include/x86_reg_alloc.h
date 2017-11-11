#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "x86_reg.h"
#include "cfg.h"

namespace mimir {
class X86RegAlloc {
 public:
  X86RegAlloc(const unsigned int virtual_reg_count);
  ~X86RegAlloc() {};

  void Allocate(const CfgNodePtr& block);

 private:
  void InitRegisters();
  X86RegPtr NextRegister(const std::string vreg);
  void CountReg(const X86InstrPtr instr);

  // Reg order contains a linear order of register names we can use to
  // index into the registers_ map.
  std::vector<std::string> reg_order_;

  // Maps a register name to a register object.
  std::unordered_map<std::string, X86RegPtr> registers_;

  // Maps an assigned vreg name to a register object.
  std::unordered_map<std::string, X86RegPtr> assigned_registers_;

  // Maps a vreg name to a count of its appearances within a block.
  std::unordered_map<std::string, unsigned int> reg_cnts_;

  unsigned int virtual_reg_count_;
};
}
