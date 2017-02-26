#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include "ast.h"
#include "symbol_table.h"
#include "lexer.h"
#include "parser.h"
#include "ir_gen.h"
#include "ir_instr.h"
#include "cfg_gen.h"
#include "cfg.h"
#include "local_optimizer.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Please provide a filename!" << std::endl;
    std::cout << "Usage: mimir {filename}" << std::endl;
    exit(1);
  }

  // Parse command line options.
  std::string filename = argv[1];

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

  LocalOptimizer lo;
  lo.Lvn(cfg.GetRoot());

  // TODO: Convert the ir to valid x86 and write to a file using
  // code_gen


  return 0;
}
