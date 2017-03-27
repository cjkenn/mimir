#include <stdlib.h>
#include "x86_section.h"
#include "x86_writer.h"

X86Writer::X86Writer() {
  filename_ = "mimir.asm";
  ofs_.open(filename_.c_str(), std::ofstream::out | std::ofstream::trunc);
}

X86Writer::X86Writer(std::string filename) {
  filename_ = filename;
  ofs_.open(filename_.c_str(), std::ofstream::out | std::ofstream::trunc);
}

X86Writer::~X86Writer() {
  ofs_.close();
}

void X86Writer::Write(const CfgNodePtr& block) {
  // 1. Create sections
  X86SectionPtr bss = std::make_shared<X86Section>("bss");
  X86SectionPtr data = std::make_shared<X86Section>("data");
  X86SectionPtr text = std::make_shared<X86Section>("text");

  // 2. Insert proper instr into start of text section (_start label,
  //    as well as stack creation)
  InitTextSection(text);

  // 3. Traverse block and all connected blocks, adding strings
  //    to the proper vectors located in each section

  // 4. Add the exit interrupt instructions to the end of the text section
  CloseTextSection(text);

  // 5. Write the bss, then data, then text section by iterating their
  //    instruction vectors and calling << ... << std::endl
  for (auto instr : bss->GetInstrs()) {
    ofs_ << instr << std::endl;
  }

  for (auto instr : data->GetInstrs()) {
    ofs_ << instr << std::endl;
  }

  for (auto instr : text->GetInstrs()) {
    ofs_ << instr << std::endl;
  }

  ofs_.close();

}

void X86Writer::InitTextSection(const X86SectionPtr& text) {
  text->InsertInstr("global _start");
  text->InsertInstr("_start:");
  text->InsertInstr("push ebp");
  text->InsertInstr("move ebp, esp");
  // TODO: Sub from esp the amount of space needed (probably need to get that from sym table)
}

void X86Writer::CloseTextSection(const X86SectionPtr& text) {
  text->InsertInstr("mov esp, ebp");
  text->InsertInstr("pop ebp");
  text->InsertInstr("mov rax, 1");
  text->InsertInstr("int 0x80");
}
