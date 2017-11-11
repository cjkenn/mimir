#include <assert.h>
#include "x86_section.h"

using namespace mimir;

X86Section::X86Section(X86SectionType type) {
  type_ = type;
  name_ = GetNameFromType();
}

void X86Section::InsertInstr(const std::string instr) {
  instrs_.push_back(instr);
}

void X86Section::InsertLabel(const std::string label_name) {
  instrs_.push_back(label_name + ":");
}

std::string X86Section::GetNameFromType() {
  switch(type_) {
  case X86SectionType::DATA_SEC:
    return "data";
  case X86SectionType::BSS_SEC:
    return "bss";
  case X86SectionType::TEXT_SEC:
    return "text";
  default:
    assert(false);
  }
}
