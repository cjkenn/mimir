#pragma once

#include <string>

namespace mimir {
enum X86InstrArgType {
  REG_X86,
  RESERVED_REG_X86,
  NUM_X86,
  ADDR_X86,
  LBL_X86
};

class X86InstrArg {
 public:
  X86InstrArg(X86InstrArgType type, std::string val);
  ~X86InstrArg() {};

  X86InstrArgType GetType() const { return type_; }
  std::string GetVal() const { return val_; }

  void SetType(X86InstrArgType t) { type_ = t; }
  void SetVal(std::string v) { val_ = v; }

 private:
  X86InstrArgType type_;
  std::string val_;
};
}
