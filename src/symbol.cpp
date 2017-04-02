#include <string>
#include "symbol.h"

Symbol::Symbol() {
  initialized_ = false;
  size_ = 8;
  stack_offset_ = 0;
}

Symbol::Symbol(std::string name) {
  name_ = name;
  initialized_ = false;
  size_ = 8;
  stack_offset_ = 0;
}
