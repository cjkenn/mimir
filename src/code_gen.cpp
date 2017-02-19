#include <stdlib.h>
#include <string>
#include "code_gen.h"
#include "ir_instr.h"

CodeGen::CodeGen(std::string filename) {
  ofs_.open(filename.c_str(), std::ofstream::out);
}

CodeGen::~CodeGen() {
  ofs_.close();
}

void CodeGen::Gen(std::vector<IrInstrPtr> ir) {

}

void CodeGen::GenInitialSections() {

}

void GenProgramExit() {

}
