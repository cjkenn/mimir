#include <stdlib.h>
#include <queue>
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
  // TODO: This block traversal is getting tedious... A nice way to do it would
  // be to provide a traverse function in cfg that takes in a function that is called
  // every time we visit a node. Then we could just call it like Traverse(ConvertX86InstrToStr())
  // or something
  ResetCfg(block);

  std::queue<CfgNodePtr> q;
  q.push(block);

  while(!q.empty()) {
    auto block = q.front();
    q.pop();
    block->SetVisited(true);
    AddInstrsToSections(bss, data, text, block);

    for (auto b : block->GetAdj()) {
      if (!b->GetVisited()) {
	q.push(b);
      }
    }
  }

  // 4. Add the exit interrupt instructions to the end of the text section
  CloseTextSection(text);

  // 5. Write the bss, then data, then text section by iterating their
  //    instruction vectors and calling << ... << std::endl
  ofs_ << "section.bss" << std::endl;
  for (auto instr : bss->GetInstrs()) {
    ofs_ << instr << std::endl;
  }

  ofs_ << "section.data" << std::endl;
  for (auto instr : data->GetInstrs()) {
    ofs_ << instr << std::endl;
  }

  ofs_ << "section.text" << std::endl;
  for (auto instr : text->GetInstrs()) {
    ofs_ << instr << std::endl;
  }

  ofs_.close();

}

void X86Writer::InitTextSection(const X86SectionPtr& text) {
  text->InsertInstr("global _start");
  text->InsertInstr("_start:");
  text->InsertInstr("push rbp");
  text->InsertInstr("move rbp, rsp");
  // TODO: Sub from esp the amount of space needed (probably need to get that from sym table)
}

void X86Writer::CloseTextSection(const X86SectionPtr& text) {
  text->InsertInstr("mov rsp, rbp");
  text->InsertInstr("pop rbp");
  text->InsertInstr("mov rax, 1");
  text->InsertInstr("int 0x80");
}

void X86Writer::AddInstrsToSections(const X86SectionPtr& bss,
				    const X86SectionPtr& data,
				    X86SectionPtr& text,
				    const CfgNodePtr& block) {
  // TODO: At this point, we're only writing to text section. In the future,
  // we can probably add helper methods to x86instr to determine which section they should
  // go into.
  for (auto instr : block->GetX86Instrs()) {
    std::string instr_str = instr->Serialize();
    text->InsertInstr(instr_str);
  }
}

void X86Writer::ResetCfg(const CfgNodePtr& block) {
  std::queue<CfgNodePtr> q;
  q.push(block);

  while (!q.empty()) {
    auto node = q.front();
    q.pop();
    node->SetVisited(false);
    for (auto r : node->GetAdj()) {
      // If we have a visited node, make sure we reset it to false.
      if (r->GetVisited()) {
	q.push(r);
      }
    }
  }
}
