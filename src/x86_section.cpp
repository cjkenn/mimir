#include "x86_section.h"

X86Section::X86Section(const std::string name) {
  name_ = name;
}

void X86Section::InsertInstr(const std::string instr) {
  instrs_.push_back(instr);
}

void X86Section::InsertLabel(const std::string label_name) {
  instrs_.push_back(label_name + ":");
}
