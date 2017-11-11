#include <stdlib.h>
#include <queue>
#include <iostream>
#include <assert.h>
#include "x86_section.h"
#include "symbol_table.h"
#include "x86_writer.h"
#include "x86_instr_sel.h"

using namespace mimir;

const std::string DEFAULT_FILENAME = "mimir.asm";
const std::string BSS = "bss";
const std::string DATA = "data";
const std::string TEXT = "text";
const std::string BITS_64 = "bits 64";
const std::string BSS_SECTION = "section .bss";
const std::string DATA_SECTION = "section .data";
const std::string TEXT_SECTION = "section .text";
const std::string SYS_INTERRUPT = "int 0x80";

X86Writer::X86Writer(std::shared_ptr<SymbolTable> sym_tab) {
  InitSections();
  sym_tab_ = sym_tab;
  filename_ = DEFAULT_FILENAME;
  ofs_.open(filename_.c_str(), std::ofstream::out | std::ofstream::trunc);
}

X86Writer::X86Writer(std::shared_ptr<SymbolTable> sym_tab, std::string filename) {
  InitSections();
  sym_tab_ = sym_tab;
  filename_ = filename;
  ofs_.open(filename_.c_str(), std::ofstream::out | std::ofstream::trunc);
}

X86Writer::~X86Writer() {
  ofs_.close();
}

void X86Writer::Write(const CfgNodePtr& block) {
  // Set 64 bits mode. Insert proper instr into start of text section
  // (_start label, as well as stack creation)
  Init64Bits();
  InitTextSection();

  // Traverse block and all connected blocks, adding strings
  // to the proper vectors located in each section
  // TODO: This block traversal is getting tedious... A nice way to do it would
  // be to provide a traverse function in cfg that takes in a function that is called
  // every time we visit a node. Then we could just call it like Traverse(ConvertX86InstrToStr())
  // or something
  std::queue<CfgNodePtr> q;
  q.push(block);

  while(!q.empty()) {
    auto block = q.front();
    q.pop();
    block->SetWriterVisited(true);
    AddInstrsToSections(block);

    for (auto b : block->GetAdj()) {
      if (!b->GetWriterVisited()) {
	q.push(b);
      }
    }
  }

  // Add the exit interrupt instructions to the end of the text section
  CloseTextSection();

  // Write the bss, then data, then text section by iterating their
  // instruction vectors and calling << ... << std::endl
  WriteSection(BSS_SECTION, bss_);
  WriteSection(DATA_SECTION, data_);
  WriteSection(TEXT_SECTION, text_);

  ofs_.close();
}

void X86Writer::Init64Bits() {
  ofs_ << BITS_64 << std::endl;
}

void X86Writer::InitTextSection() {
  text_->InsertInstr("global _start");
  text_->InsertInstr("_start:");
  text_->InsertInstr(PushRbpInstr());
  text_->InsertInstr(MovRbpRspInstr());
  const int stack_space = sym_tab_->GetSizeOfCurrentScope();
  const std::string st = "sub rsp, " + std::to_string(stack_space);
  text_->InsertInstr(st);
}

void X86Writer::CloseTextSection() {
  text_->InsertInstr(MovRspRbpInstr());
  text_->InsertInstr(PopRbpInstr());
  text_->InsertInstr(MovExitInstr());
  text_->InsertInstr(SYS_INTERRUPT);
}

void X86Writer::AddInstrsToSections(const CfgNodePtr& block) {
  // TODO: At this point, we're only writing to text section. In the future,
  // we can probably add helper methods to x86instr to determine which section they should
  // go into.
  const auto instrs = block->GetX86Instrs();
  if (!instrs.empty()) {
    text_->InsertLabel(instrs[0]->GetLabel());
  }

  for (unsigned int i = 0; i < instrs.size(); i++) {
    if ((i != 0) && instrs[i]->GetLabel() != instrs[i-1]->GetLabel()) {
      text_->InsertLabel(instrs[i]->GetLabel());
    }

    std::string instr_str = instrs[i]->Serialize();
    text_->InsertInstr(instr_str);
  }
}

void X86Writer::WriteSection(const std::string section_name, const X86SectionPtr& section) {
  if (!section->GetInstrs().empty()) {
    ofs_ << section_name << std::endl;
    for (auto instr : section->GetInstrs()) {
      ofs_ << instr << std::endl;
    }
  }
}

void X86Writer::InitSections() {
  bss_ = std::make_shared<X86Section>(X86SectionType::BSS_SEC);
  data_ = std::make_shared<X86Section>(X86SectionType::DATA_SEC);
  text_ = std::make_shared<X86Section>(X86SectionType::TEXT_SEC);
}

std::string X86Writer::PushRbpInstr() {
  return "push rbp";
}

std::string X86Writer::MovRbpRspInstr() {
  return "mov rbp, rsp";
}

std::string X86Writer::MovRspRbpInstr() {
  return "mov rsp, rbp";
}

std::string X86Writer::PopRbpInstr() {
  return "pop rbp";
}

std::string X86Writer::MovExitInstr() {
  return "mov rax, 1";
}
