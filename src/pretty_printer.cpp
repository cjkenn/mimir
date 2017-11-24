#include <vector>
#include <string>
#include <iostream>
#include <assert.h>
#include "pretty_printer.h"
#include "ast.h"
#include "ir_instr.h"
#include "token.h"

using namespace mimir;

const unsigned int NUM_AST_TYPES = 26;
const unsigned int NUM_IR_TYPES = 21;
const unsigned int NUM_TKN_TYPES = 27;

// The following static arrays are a bit of a nightmare for maintenance. The order
// of the initialized elements must matcht the order of the types in each respective
// enum. When a new type is added to tkn, ast, or ir, it must be inserted correctly
// in both places, and the constants above should be updated.
// It's probably better to make this into a macro maybe?

const std::string PrettyPrinter::tkn_map[] = {
  "(",
  ")",
  "{",
  "}",
  "+",
  "-",
  "*",
  "/",
  "%",
  "!",
  ";",
  ",",
  "=",
  "==",
  "!=",
  "<",
  "<=",
  ">",
  ">=",
  "Identifier",
  "Number",
  "'If' keyword",
  "'Else' keyword",
  "'While' keyword",
  "'Function' keyword",
  "Unknown token",
  "EOF"
};

const std::string PrettyPrinter::ast_map[] = {
  "Var",
  "Constant",
  "Addition",
  "Subtraction",
  "Multiplication",
  "Division",
  "Mod",
  "Equality",
  "Not equal",
  "Less than",
  "Greater than",
  "Less equal",
  "Greater equal",
  "Set",
  "If",
  "Else",
  "While",
  "Function decl",
  "Function call",
  "Function params",
  "Statement sequence",
  "Expression",
  "Negation",
  "Program",
  "Empty",
  "EOF"
};

const std::string PrettyPrinter::ir_map[] = {
  "MV",
  "LD",
  "SV",
  "JMP",
  "JMPLT",
  "JMPLTE",
  "JMPGT",
  "JMPGTE",
  "JMPEQ",
  "JMPNEQ",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "MOD",
  "NOP",
  "CMP",
  "FUNC_ENTER",
  "FUNC_EXIT",
  "FUNC_CALL",
  "FUNC_PARAM"
};

static_assert(sizeof(PrettyPrinter::ast_map)/sizeof(PrettyPrinter::ast_map[0]) == NUM_AST_TYPES,
	      "ast size mismatch");
static_assert(sizeof(PrettyPrinter::ir_map)/sizeof(PrettyPrinter::ir_map[0]) == NUM_IR_TYPES,
	      "ir size mismatch");
static_assert(sizeof(PrettyPrinter::tkn_map)/sizeof(PrettyPrinter::tkn_map[0]) == NUM_TKN_TYPES,
	      "tkn size mismatch");

void PrettyPrinter::PrintSingleAst(const AstNodePtr ast) {
  std::string output = ast_map[ast->GetType()];
  if (ast->GetVal()) {
    output += ": ";
    output += std::to_string(ast->GetVal());
  } else if (ast->GetText() != "") {
    output += ": ";
    output += ("(" + ast->GetText() + ")");
  }

  std::cout << output << std::endl;
}

void PrettyPrinter::PrintSingleIr(const IrInstrPtr ir) {
  std::string output = ir_map[ir->GetType()];

  if (ir->GetArgs().first != "") {
    output += " -> ";
    output += ir->GetArgs().first;
  }

  if (ir->GetArgs().second != "") {
    output += " = ";
    output += ir->GetArgs().second;
  }

  std::cout << output << std::endl;
}

void PrettyPrinter::PrintTkn(const Token tkn) {
  std::string output = tkn_map[tkn.GetType()];

  if (tkn.GetVal()) {
    output += "(";
    output += tkn.GetVal();
    output += ")";
  } else if (tkn.GetText() != "") {
    output += "(";
    output += tkn.GetText();
    output += ")";
  } else {
    output += "()";
  }

  output += " : ";
  output += ("L" + tkn.GetLinePos());
  output += ("C" + tkn.GetCharPos());

  std::cout << output << std::endl;
}

void PrettyPrinter::PrintAst(const AstNodePtr ast, unsigned int level) {
  if (ast == nullptr) {
    return;
  }

  // This isn't the prettiest or even correct way to print everything,
  // but it's good enough for this purpose. We really need this to verify the structure
  // of the ast and ensure that nodes are in the place we expect them to be.
  const int spaces = level * 3;
  std::string prntspaces;
  for (int i = 0; i < spaces-1; i++) {
    prntspaces += ' ';
  }

  std::string output = ('(' + ast_map[ast->GetType()] + ')');

  if (ast->GetVal()) {
    output += ":";
    output += ('(' + std::to_string(ast->GetVal()) + ')');
  } else if (ast->GetText() != "") {
    output += ":";
    output += ("(" + ast->GetText() + ")");
  }

  std::cout << "|";
  std::cout << prntspaces;
  std::cout << output << std::endl;

  for (auto node : ast->GetChildren()) {
    std::cout << "|";
    std::cout << prntspaces;

    if (level != 0) {
      std::cout << "|";
    }

    std::cout << "__" << std::endl;

    PrintAst(node, level+1);
  }
}

void PrettyPrinter::PrintIr(const std::vector<IrInstrPtr> ir) {
  for (unsigned int i = 0; i < ir.size(); i++) {
    std::cout << i << ": ";
    PrintSingleIr(ir[i]);
  }
}
