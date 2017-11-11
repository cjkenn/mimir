#pragma once

#include <string>
#include <vector>
#include <memory>

namespace mimir {
class X86Section;

typedef std::shared_ptr<X86Section> X86SectionPtr;

enum X86SectionType {
  DATA_SEC,
  BSS_SEC,
  TEXT_SEC,
};

class X86Section {
 public:
  X86Section(X86SectionType type);
  ~X86Section() {};

  std::string GetName() const { return name_; }
  std::vector<std::string> GetInstrs() const { return instrs_; }
  X86SectionType GetType() const { return type_; }

  std::string GetNameFromType();
  void InsertInstr(const std::string instr);
  void InsertLabel(const std::string label_name);

 private:
  X86SectionType type_;
  std::string name_;
  std::vector<std::string> instrs_;
};
}
