#include <string>
#include "x86_instr_arg.h"

using namespace mimir;

X86InstrArg::X86InstrArg(X86InstrArgType type, std::string v) {
  type_ = type;
  val_ = v;
}
