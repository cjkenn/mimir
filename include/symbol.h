#pragma once

#include <string>
#include <memory>

namespace mimir {
class Symbol;

typedef std::shared_ptr<Symbol> SymbolPtr;

class Symbol {
 public:
  Symbol();
  Symbol(std::string name);
  ~Symbol() {};

  std::string GetName() const { return name_; }
  std::string GetType() const { return type_; }
  int GetStackOffset() const { return stack_offset_; }
  int GetScopeLevel() const { return scope_level_; }
  bool IsInitialized() const { return initialized_; }

  void SetName(std::string name) { name_ = name; }
  void SetStackOffset(int so) { stack_offset_ = so; }
  void SetScopeLevel(int level) { scope_level_ = level; }
  bool IsGlobalScope() const { return scope_level_ == 0; }
  void Initialize() { initialized_ = true; }

 private:
  std::string name_;
  std::string type_;
  bool initialized_;

  // TODO: Should eventually be replaced by type stuff. For now,
  // we only support number types so this is always 4.
  int size_;

  // Determined by the number of variables encountered in the program,
  // this will be used to store and load vars during code gen.
  int stack_offset_;

  // The nesting level of the scope of this symbol. 0 is defined as global scope.
  int scope_level_;
};
}
