#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <assert.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ir_gen.h"
#include "symbol_table.h"
#include "cfg_gen.h"
#include "cfg.h"
#include "x86_instr_sel.h"
#include "x86_writer.h"

void verify_single_assign() {
  std::ifstream ifs("output/single_assign");
  std::string line;

  std::vector<std::string> expected_instrs;
  expected_instrs.push_back("bits 64");
  expected_instrs.push_back("section .text");
  expected_instrs.push_back("global _start");
  expected_instrs.push_back("_start:");
  expected_instrs.push_back("push rbp");
  expected_instrs.push_back("mov rbp, rsp");
  expected_instrs.push_back("sub rsp, 8");
  expected_instrs.push_back("lbl0:");
  expected_instrs.push_back("mov r0, 11");
  expected_instrs.push_back("mov [rbp-8], r0");
  expected_instrs.push_back("nop ");
  expected_instrs.push_back("mov rsp, rbp");
  expected_instrs.push_back("pop rbp");
  expected_instrs.push_back("mov rax, 1");
  expected_instrs.push_back("int 0x80");
  int counter = 0;

  while (std::getline(ifs, line)) {
    assert(expected_instrs[counter] == line);
    counter++;
  }

}

void verify(const std::string name) {
  if (name == "single_assign") {
    verify_single_assign();
  }
}

void test_x86_gen(const std::string filename) {
  // Make lexer and parser, parse input file
  auto lexer = std::make_shared<Lexer>("input/" + filename);
  auto sym_tab = std::make_shared<SymbolTable>();
  Parser parser(lexer, sym_tab);
  const ParserResult parser_result = parser.Parse();

  const AstNodePtr ast = parser_result.GetAst();

  IrGen ir_gen;
  const std::vector<IrInstrPtr> ir = ir_gen.Gen(ast);

  CfgGen cfg_gen;
  Cfg cfg = cfg_gen.Gen(ir);

  X86InstrSel x86_sel(sym_tab);
  x86_sel.SelectInstrsForEntireBranch(cfg.GetRoot());

  const std::string output_file = "output/" + filename;
  X86Writer x86_writer(sym_tab, output_file);
  x86_writer.Write(cfg.GetRoot());

  // Verify output file
  verify(filename);
}

int main(int argc, char **argv) {
  test_x86_gen("single_assign");
  std::cout << "x86 gen single assign test passed!" << std::endl;

  return 0;
}
