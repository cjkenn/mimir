#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
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
#include "compiler_opts.h"

int main(int argc, char **argv) {
  CompilerOptions opts;
  opts.ParseCommandLine(argc, argv);

  if (opts.HasCommandLineErrors()) {
    opts.ReportCommandLineErrors();
    return -1;
  }

  const std::string filename = opts.GetInputFilename();

  // Lex and parse the input file, storing variables
  // in our symbol table.
  auto lexer = std::make_shared<Lexer>(filename);
  auto sym_tab = std::make_shared<SymbolTable>();
  Parser parser(lexer, sym_tab);
  ParserResult parser_result = parser.Parse();

  // If the parser contains any errors, we don't continue
  // with analysis/ generation. Show the user the errors
  // and return.
  if (parser_result.HasError()) {
    parser_result.ReportErrors();
    return -1;
  }

  AstNodePtr ast = parser_result.GetAst();

  // Convert the ast to ir for analysis and x86 generation.
  IrGen ir_gen;
  std::vector<IrInstrPtr> ir = ir_gen.Gen(ast);

  // Cfg construction and analysis
  CfgGen cfg_gen;
  Cfg cfg = cfg_gen.Gen(ir);

  if (opts.ShouldPerformLocalOptimization()) {
    LocalOptimizer lo;
    auto root = cfg.GetRoot();
    lo.OptimizeBlock(root);
  }

  X86Writer x86_writer(sym_tab);
  x86_writer.Write(cfg.GetRoot());

  return 0;
}
