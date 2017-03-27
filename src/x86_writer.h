#pragma once

#include <fstream>
#include <string>
#include "cfg_node.h"
#include "x86_section.h"

class X86Writer {
 public:
  X86Writer();
  X86Writer(std::string filename);
  ~X86Writer();

  void Write(const CfgNodePtr& block);
 private:
  void InitTextSection(const X86SectionPtr& text);
  void CloseTextSection(const X86SectionPtr& text);

  std::string filename_;
  std::ofstream ofs_;
};
