#pragma once

#include <string>
#include <vector>
#include <memory>

class X86Section;

typedef std::shared_ptr<X86Section> X86SectionPtr;

class X86Section {
 public:
  X86Section(std::string name);
  ~X86Section() {};

  std::string GetName() const { return name_; }
  std::vector<std::string> GetInstrs() const { return instrs_; }

  void InsertInstr(const std::string instr);
 private:
  std::string name_;
  std::vector<std::string> instrs_;
};
