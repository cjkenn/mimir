#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "ir_instr.h"

class CodeGen {
 public:
  CodeGen(std::string filename);
  ~CodeGen();

  void Gen(std::vector<IrInstrPtr> ir);
 private:
  void GenInitialSections();
  void GenProgramExit();

  std::ofstream ofs_;
};
