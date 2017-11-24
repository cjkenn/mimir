#include <vector>
#include <string>
#include <iostream>
#include "pretty_printer.h"
#include "ast.h"
#include "ir_instr.h"
#include "token.h"

using namespace mimir;

PrettyPrinter::PrettyPrinter() {
  InitTknMap();
  InitAstMap();
  InitIrMap();
}

void PrettyPrinter::PrintSingleAst(const AstNodePtr ast) {
  std::string output = ast_map_[ast->GetType()];
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
  std::string output = ir_map_[ir->GetType()];

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
  std::string output = tkn_map_[tkn.GetType()];

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

  std::string output = ('(' + ast_map_[ast->GetType()] + ')');

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

void PrettyPrinter::InitTknMap() {
  tkn_map_[TokenType::ID_TKN] = "Identifier";
  tkn_map_[TokenType::NUM_TKN] = "Number";
  tkn_map_[TokenType::IF_TKN] = "'If' keyword";
  tkn_map_[TokenType::ELSE_TKN] = "'Else' keyword";
  tkn_map_[TokenType::WHILE_TKN] = "'While' keyword";
  tkn_map_[TokenType::FUNC_TKN] = "'Function' keyword";
  tkn_map_[TokenType::LEFT_PAREN_TKN] = "(";
  tkn_map_[TokenType::RIGHT_PAREN_TKN] = ")";
  tkn_map_[TokenType::LEFT_BRACE_TKN] = "{";
  tkn_map_[TokenType::RIGHT_BRACE_TKN] = "}";
  tkn_map_[TokenType::PLUS_TKN] = "+";
  tkn_map_[TokenType::MINUS_TKN] = "-";
  tkn_map_[TokenType::STAR_TKN] = "*";
  tkn_map_[TokenType::BACKSLASH_TKN] = "/";
  tkn_map_[TokenType::PERCENT_TKN] = "%";
  tkn_map_[TokenType::SEMICOLON_TKN] = ";";
  tkn_map_[TokenType::COMMA_TKN] = ",";
  tkn_map_[TokenType::EQ_TKN] = "=";
  tkn_map_[TokenType::LT_TKN] = "<";
  tkn_map_[TokenType::LTE_TKN] = "<=";
  tkn_map_[TokenType::GT_TKN] = ">";
  tkn_map_[TokenType::GTE_TKN] = ">=";
  tkn_map_[TokenType::EQEQ_TKN] = "==";
  tkn_map_[TokenType::EOF_TKN] = "EOF";
}

void PrettyPrinter::InitAstMap() {
  ast_map_[AstType::VAR_AST] = "Var";
  ast_map_[AstType::CST_AST] = "Constant";
  ast_map_[AstType::ADD_AST] = "Addition";
  ast_map_[AstType::SUB_AST] = "Subtraction";
  ast_map_[AstType::MUL_AST] = "Multiplication";
  ast_map_[AstType::DIV_AST] = "Division";
  ast_map_[AstType::MOD_AST] = "Mod";
  ast_map_[AstType::EQEQ_AST] = "Equality";
  ast_map_[AstType::NEQ_AST] = "Not equal";
  ast_map_[AstType::LT_AST] = "Less than";
  ast_map_[AstType::GT_AST] = "Greater than";
  ast_map_[AstType::LTE_AST] = "Less equal";
  ast_map_[AstType::GTE_AST] = "Greater equal";
  ast_map_[AstType::SET_AST] = "Set";
  ast_map_[AstType::IF_AST] = "If";
  ast_map_[AstType::ELSE_AST] = "Else";
  ast_map_[AstType::WHILE_AST] = "While";
  ast_map_[AstType::FUNC_AST] = "Function decl";
  ast_map_[AstType::FUNC_CALL_AST] = "Function call";
  ast_map_[AstType::PARAMS_AST] = "Function params";
  ast_map_[AstType::SEQ_AST] = "Statement sequence";
  ast_map_[AstType::EXPR_AST] = "Expression";
  ast_map_[AstType::NEG_AST] = "Negation";
  ast_map_[AstType::PROG_AST] = "Program";
  ast_map_[AstType::EMPTY_AST] = "Empty";
  ast_map_[AstType::EOF_AST] = "EOF";
}

void PrettyPrinter::InitIrMap() {
  ir_map_[IrInstrType::MV_INSTR] = "MV";
  ir_map_[IrInstrType::LD_INSTR] = "LD";
  ir_map_[IrInstrType::SV_INSTR] = "SV";
  ir_map_[IrInstrType::JMP_INSTR] = "JMP";
  ir_map_[IrInstrType::JMPLT_INSTR] = "JMPLT";
  ir_map_[IrInstrType::JMPLTE_INSTR] = "JMPLTE";
  ir_map_[IrInstrType::JMPGT_INSTR] = "JMPGT";
  ir_map_[IrInstrType::JMPGTE_INSTR] = "JMPGTE";
  ir_map_[IrInstrType::JMPEQ_INSTR] = "JMPEQ";
  ir_map_[IrInstrType::JMPNEQ_INSTR] = "JMPNEQ";
  ir_map_[IrInstrType::ADD_INSTR] = "ADD";
  ir_map_[IrInstrType::SUB_INSTR] = "SUB";
  ir_map_[IrInstrType::MUL_INSTR] = "MUL";
  ir_map_[IrInstrType::DIV_INSTR] = "DIV";
  ir_map_[IrInstrType::MOD_INSTR] = "MOD";
  ir_map_[IrInstrType::NOP_INSTR] = "NOP";
  ir_map_[IrInstrType::CMP_INSTR] = "CMP";
  ir_map_[IrInstrType::FUNC_ENTER_INSTR] = "FUNC_ENTER";
  ir_map_[IrInstrType::FUNC_EXIT_INSTR] = "FUNC_EXIT";
  ir_map_[IrInstrType::FUNC_CALL_INSTR] = "FUNC_CALL";
  ir_map_[IrInstrType::FUNC_PARAM_INSTR] = "FUNC_PARAM";
}
