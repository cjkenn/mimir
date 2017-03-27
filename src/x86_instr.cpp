#include "x86_instr.h"

X86Instr::X86Instr(X86InstrType type) {
  type_ = type;
}

X86Instr::X86Instr(X86InstrType type, std::string label) {
  type_ = type;
  label_ = label;
}
