#pragma once

#include <string>
#include <memory>

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

  void SetName(std::string name) { name_ = name; }
  void SetStackOffset(int so) { stack_offset_ = so; }

 private:
  std::string name_;
  std::string type_;
  // TODO: Should eventually be replaced by type stuff. For now,
  // we only support number types so this is always 4.
  int size_;
  // Determined by the number of variables encountered in the program,
  // this will be used to store and load vars during code gen.
  int stack_offset_;
};
