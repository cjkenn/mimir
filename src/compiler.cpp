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


// TODO: bandaid for now. Really could have separate pointers for visitation
// so we don't need to reset a visited field.
void resetCfg(const CfgNodePtr& block) {
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
  const int virtual_reg_count = ir_gen.GetRegCount();

  // Cfg construction and analysis
  CfgGen cfg_gen;
  Cfg cfg = cfg_gen.Gen(ir);

  if (opts.ShouldPerformLocalOptimization()) {
    LocalOptimizer lo;
    auto root = cfg.GetRoot();
    lo.OptimizeBlock(root);
  }

  // Instr selection: convert ir to x86
  X86InstrSel x86_sel(sym_tab);
  x86_sel.SelectInstrsForEntireBranch(cfg.GetRoot());

  resetCfg(cfg.GetRoot());

  // Register allocation: convert virtual ir registers to x86 registers
  X86RegAlloc x86_alloc(virtual_reg_count);
  x86_alloc.Allocate(cfg.GetRoot());

  resetCfg(cfg.GetRoot());

  // Writing: Walk x86 instructions in ast and write nasm x86 to output file
  X86Writer x86_writer(sym_tab);
  x86_writer.Write(cfg.GetRoot());

  return 0;
}
