#pragma once

#include <vector>
#include <utility>
#include <string>
#include <memory>

namespace mimir {
class IrInstr;

typedef std::shared_ptr<IrInstr> IrInstrPtr;

enum IrInstrType {
  MV_INSTR,
  LD_INSTR,
  SV_INSTR,
  JMP_INSTR,
  JMPLT_INSTR,
  JMPLTE_INSTR,
  JMPGT_INSTR,
  JMPGTE_INSTR,
  JMPEQ_INSTR,
  JMPNEQ_INSTR,
  ADD_INSTR,
  SUB_INSTR,
  MUL_INSTR,
  DIV_INSTR,
  MOD_INSTR,
  NOP_INSTR,
  CMP_INSTR,
  FUNC_ENTER_INSTR,
  FUNC_EXIT_INSTR,
  FUNC_CALL_INSTR,
  FUNC_PARAM_INSTR
};

class IrInstr {
 public:
  IrInstr() {};
  IrInstr(IrInstrType type);
  IrInstr(IrInstrType type,
	  std::pair<std::string, std::string> args,
	  std::string dest);
  IrInstr(IrInstrType type,
	  std::string arg1,
	  std::string arg2,
	  std::string dest);
  ~IrInstr() {};

  IrInstrType GetType() const { return type_; }
  std::string GetDest() const { return dest_; }
  std::pair<std::string, std::string> GetArgs() const { return args_; }
  std::string GetLabel() const { return label_; }
  bool GetIsLeader() const { return is_leader_; }
  bool IsRedundant() const { return redundant_; }
  bool IsConstantOp() const { return constant_op_; }
  int GetConstantVal() const { return constant_val_; }

  void SetType(IrInstrType type) { type_ = type; }
  void SetArgs(std::pair<std::string, std::string> args) { args_ = args; }
  void SetDest(std::string dest) { dest_ = dest; }
  void SetLabel(std::string l) { label_ = l; }
  void SetIsLeader(bool is_leader) { is_leader_ = is_leader; }
  void SetRedundant(bool redundant) { redundant_ = redundant; }
  void SetConstantOp(bool constant_op) { constant_op_ = constant_op; }
  void SetConstantVal(int constant_val) { constant_val_ = constant_val; }

  // True if the instruction is a jump.
  bool IsJmp();

  // True if the instruction is a binary operator.
  bool IsBinOp();

  // True if the instruction is a function definition.
  bool IsFunc();

  // True if the instruction is a function call.
  bool IsCall();

  // Returns the instruction type as a string.
  const std::string GetTypeAsStr();

  // True if the instruction is a commutative binary operator.
  bool IsCommutative();

  // Helper function that is called during constant folding optimization.
  // This will convert the instruction from an expression to a mov of
  // a constant value, which should be the result of evaluating the original
  // expression (this only occurs if the expression contains only constant
  // values, ie. 5 + 2).
  void ConvertConstant();

 private:
  // The type of instruction. This roughly corresponds to an opcode
  IrInstrType type_;

  // The pair or arguments given to this instruction. If the intstruction
  // only uses on argument (like sv), this is put into the
  // dest_ variable, and the args are left as an empty pair. If the instruction
  // is a jmp, the args will be empty.
  std::pair<std::string, std::string> args_;

  // The destination of the instruction. This means the register that the result
  // will be stored in.
  std::string dest_;

  // The label under which this instruction sits. This field is set for every
  // instruction, not just the first one in which a label appears. We can tell
  // when a label changes by checking if the next instruction has a different
  // label value.
  std::string label_;

  // Whether this instruction is the leader of a block or not. An instruction
  // is called a leader if it is the first instruction in a procedure, or it
  // has a label that is a potential jump target.
  bool is_leader_;

  // Instructions can be marked as redundant during optimization phases. Depending
  // on the optimization, this might mean the instruction can be deleted or moved.
  bool redundant_;

  // Flag indicating that this instruction is a constant operation, and thus can
  // be altered during optimizations.
  bool constant_op_;

  // The constant value that this instruction actually evaluates to
  int constant_val_;
};
}
