#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <queue>
#include "ast.h"
#include "symbol_table.h"
#include "lexer.h"
#include "parser.h"
#include "ir_gen.h"
#include "ir_instr.h"
#include "cfg_gen.h"
#include "cfg.h"
#include "local_optimizer.h"
#include "x86_writer.h"
#include "x86_instr_sel.h"
#include "x86_reg_alloc.h"
#include "compiler_opts.h"
#include "pretty_printer.h"

int main(int argc, char **argv) {
  mimir::CompilerOptions opts;
  opts.ParseCommandLine(argc, argv);

  if (opts.HasCommandLineErrors()) {
    opts.ReportCommandLineErrors();
    return -1;
  }

  const std::string filename = opts.GetInputFilename();

  // Lex and parse the input file, storing variables
  // in our symbol table.
  auto lexer = std::make_shared<mimir::Lexer>(filename);
  auto sym_tab = std::make_shared<mimir::SymbolTable>();

  mimir::Parser parser(lexer, sym_tab);
  mimir::ParserResult parser_result = parser.Parse();

  // If the parser contains any errors, we don't continue
  // with analysis/ generation. Show the user the errors
  // and return.
  if (parser_result.HasError()) {
    parser_result.ReportErrors();
    return -1;
  }

  mimir::AstNodePtr ast = parser_result.GetAst();

  // If we want to show the AST, we print it here.
  if (opts.ShouldPrintAst()) {
    mimir::PrettyPrinter::PrintAst(ast, 0);
  }

  // Convert the ast to ir for analysis and x86 generation.
  mimir::IrGen ir_gen;
  std::vector<mimir::IrInstrPtr> ir = ir_gen.Gen(ast);
  const int virtual_reg_count = ir_gen.GetRegCount();

  // If we want to show the IR, we print it here.
  if (opts.ShouldPrintIr()) {
    mimir::PrettyPrinter::PrintIr(ir);
  }

  // Cfg construction and analysis
  mimir::CfgGen cfg_gen;
  mimir::Cfg cfg = cfg_gen.Gen(ir);

  if (opts.ShouldPerformLocalOptimization()) {
    mimir::LocalOptimizer lo;
    auto root = cfg.GetRoot();
    lo.OptimizeBlock(root);
  }

  // Instr selection: convert ir to x86
  mimir::X86InstrSel x86_sel(sym_tab);
  x86_sel.SelectInstrsForEntireBranch(cfg.GetRoot());

  // Register allocation: convert virtual ir registers to x86 registers
  mimir::X86RegAlloc x86_alloc(virtual_reg_count);
  x86_alloc.Allocate(cfg.GetRoot());

  // Writing: Walk x86 instructions in ast and write nasm x86 to output file
  mimir::X86Writer x86_writer(sym_tab);
  x86_writer.Write(cfg.GetRoot());

  return 0;
}
