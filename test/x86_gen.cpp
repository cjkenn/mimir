#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <assert.h>
#include <queue>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ir_gen.h"
#include "symbol_table.h"
#include "cfg_gen.h"
#include "cfg.h"
#include "x86_instr_sel.h"
#include "x86_writer.h"
#include "x86_reg_alloc.h"

const std::string OUTPUT_DIR = "output/";
const std::string INPUT_DIR = "input/";
const std::string EXPECTED_DIR = "expected/";

void print_success(const std::string message) {
  std::cout << "x86 gen " << message << " test passed!" << std::endl;
}

void verify(const std::string name) {
  std::ifstream output(OUTPUT_DIR + name);
  std::ifstream expected(EXPECTED_DIR + name);
  std::string output_line;
  std::string expected_line;

  while (std::getline(output, output_line) && std::getline(expected, expected_line)) {
    if (output_line != expected_line) {
      std::cout << "In file: '" << name << "' -->" << std::endl;
      std::cout << "Expected: '" << expected_line <<
	"' but found: '" << output_line << "'" <<
	std::endl;
      assert(false);
    }
  }
}

void test_x86_gen(const std::string filename) {
  auto lexer = std::make_shared<mimir::Lexer>(INPUT_DIR + filename);
  auto sym_tab = std::make_shared<mimir::SymbolTable>();

  mimir::Parser parser(lexer, sym_tab);
  const mimir::ParserResult parser_result = parser.Parse();

  const mimir::AstNodePtr ast = parser_result.GetAst();

  mimir::IrGen ir_gen;
  const std::vector<mimir::IrInstrPtr> ir = ir_gen.Gen(ast);

  mimir::CfgGen cfg_gen;
  mimir::Cfg cfg = cfg_gen.Gen(ir);

  mimir::X86InstrSel x86_sel(sym_tab);
  x86_sel.SelectInstrsForEntireBranch(cfg.GetRoot());

  mimir::X86RegAlloc x86_alloc(ir_gen.GetRegCount());
  x86_alloc.Allocate(cfg.GetRoot());

  const std::string output_file = OUTPUT_DIR + filename;
  mimir::X86Writer x86_writer(sym_tab, output_file);
  x86_writer.Write(cfg.GetRoot());

  // Verify output file
  verify(filename);
}

int main(int argc, char **argv) {
  test_x86_gen("single_assign");
  print_success("single assignment");

  test_x86_gen("multiple_assign");
  print_success("multiple assignments");

  test_x86_gen("while_loop");
  print_success("while loop");

  test_x86_gen("if_statement");
  print_success("if statement");

  test_x86_gen("if_else_statement");
  print_success("if else statement");

  test_x86_gen("expr_assign");
  print_success("expression assignment");

  return 0;
}
