#pragma once

#include <fstream>
#include <string>
#include <memory>
#include "cfg_node.h"
#include "symbol_table.h"
#include "x86_section.h"
#include "x86_reg_alloc.h"

class X86Writer {
 public:
  X86Writer(std::shared_ptr<SymbolTable> sym_tab);
  X86Writer(std::shared_ptr<SymbolTable> sym_tab, std::string filename);
  ~X86Writer();

  void Write(const CfgNodePtr& block);
 private:
  void Init64Bits();
  void InitTextSection();
  void CloseTextSection();
  void AddInstrsToSections(const CfgNodePtr& block);
  void WriteSection(const std::string section_name, const X86SectionPtr& section);
  void ResetCfg(const CfgNodePtr& block);
  void InitSections();

  std::string PushRbpInstr();
  std::string MovRbpRspInstr();
  std::string MovRspRbpInstr();
  std::string PopRbpInstr();
  std::string MovExitInstr();

  std::string filename_;
  std::ofstream ofs_;
  X86SectionPtr bss_;
  X86SectionPtr data_;
  X86SectionPtr text_;
  std::shared_ptr<SymbolTable> sym_tab_;
  X86RegAlloc allocator_;
};
