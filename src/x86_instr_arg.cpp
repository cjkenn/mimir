#include <string>
#include "x86_instr_arg.h"

X86InstrArg::X86InstrArg(X86InstrArgType type, std::string v) {
  type_ = type;
  val_ = v;
}
