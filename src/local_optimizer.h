#pragma once

#include <string>
#include <unordered_map>
#include "cfg_node.h"
#include "ir_instr.h"

class LocalOptimizer {
 public:
  LocalOptimizer();
  ~LocalOptimizer() {};

  // Perform local value numbering optimizations,
  // including removing redundant instructions
  // and constant folding. This method operates only on
  // the block provided. If local optimizations are needed
  // on all blocks, this must be called for each of them
  // (but the order of those calls is not important, since only
  // the block itself is affected).
  void OptimizeBlock(CfgNodePtr& block);

 private:
  // We only care about certain instructions for local optimizations. There
  // is no reason to analyze compares or jumps or nops, so we filter them out
  // with this method.
  bool IsInstrLvnValid(const IrInstrPtr& instr);

  // Determine if an instr is a candidate for constant folding. This method
  // will mark the instructions for deletion or alteration, to happen
  // at a later time during optimization.
  void CheckAndMarkConstantOp(const std::vector<IrInstrPtr>& instrs, const int i);

  // Evaluate an instruction if it has constant operands, given those operands.
  int EvalConstantOp(const IrInstrPtr& instr, const int val1, const int val2);

  // Check binary operations for identity functions. This function checks
  // the type of the op and delegates to a helper function to handle each one.
  void CheckAndMarkIdentities(const std::vector<IrInstrPtr>& instrs, const int i);

  // Determine the lvn value for the first argument to an instruction.
  int GetLvnForFirstArg(const IrInstrPtr& instr);

  // Similar to the above method, but with some extra checking to ensure
  // that LD instruction arguments have the correct values.
  int GetLvnForSecondArg(const IrInstrPtr& instr, int val1);

  // We must construct a string to act as the key for the op_map.
  // The key takes the form of "V1OPV2". For example,
  // the operation "add r0, r1", with v(r0) = 0 and v(r1) = 1, will result in
  // a key of "0ADD1".
  // We also want operations like a + b and b + a to have the same hash key,
  // because they are commutative operations. To account for this, the key
  // is contructed by always placing the small value number first. So, given the
  // same value numbers as above, the operation "add r1, r0" will also hash to
  // "0ADD1". Note that this is only done when the operation is commutative,
  // so only for ADD and MUL.
  std::string BuildLvnMapKey(const IrInstrPtr& instr, const int val1, const int val2);

  // Helper function to check for redundant LD instructions, assuming
  // the the instruction we pass in has been flagged as redundant
  // itself.
  void MarkPreviousLdInstrs(const std::vector<IrInstrPtr>& instrs, const int i);

  // We are only concerned with one identity for add: a + 0 = a. Note that we
  // do not try to optimize away any constant identities like 5 + 0 = 5, because
  // that's already handled by constant folding and we don't need to evaluate it
  // again. The order of optimizations does not matter here.
  void CheckAndMarkAddIdentity(const std::vector<IrInstrPtr>& instrs, const int i);

  void CheckAndMarkSubIdentity(const std::vector<IrInstrPtr>& instrs, const int i);

  void CheckAndMarkMulIdentity(const std::vector<IrInstrPtr>& instrs, const int i);

  void CheckAndMarkDivIdentity(const std::vector<IrInstrPtr>& instrs, const int i);

  // Reduce a - 0 = a to one instruction, a LD into the destination
  // of the original SUB instruction.
  void OptSubByZero(const IrInstrPtr& first_instr,
		    const IrInstrPtr& second_instr,
		    const IrInstrPtr& sub_instr);

  // Reduce a - a = 0 to a single instruction, a MV into the destination
  // of the original SUB instruction.
  void OptSubBySelf(const IrInstrPtr& first_instr,
		    const IrInstrPtr& second_instr,
		    const IrInstrPtr& sub_instr);

  // Reduce 2 x a to a + a, to avoid a more expensive MUL operation.
  void OptMulByTwo(const IrInstrPtr& first_instr,
		   const IrInstrPtr& second_instr,
		   const IrInstrPtr& mul_instr);

  // Reduce 1 x a = a to a single LD instruction.
  void OptMulByOne(const IrInstrPtr& first_instr,
		   const IrInstrPtr& second_instr,
		   const IrInstrPtr& mul_instr);

  // Reduce 0 x a = 0 to a single MV instruction.
  void OptMulByZero(const IrInstrPtr& first_instr,
		    const IrInstrPtr& second_instr,
		    const IrInstrPtr& mul_instr);

  // Reduce a / 1 = a to a single LD instruction.
  void OptDivByOne(const IrInstrPtr& first_instr,
		   const IrInstrPtr& second_instr,
		   const IrInstrPtr& div_instr);

  bool IsLdOrMvOrder(const IrInstrPtr& first_instr,
		      const IrInstrPtr& second_instr);

  bool IsLdThenMvOrder(const IrInstrPtr& first_instr,
		       const IrInstrPtr& second_instr);

  bool IsLdThenLdOrder(const IrInstrPtr& first_instr,
		       const IrInstrPtr& second_instr);

  int val_count_;
  std::unordered_map<std::string, int> val_map_;
  std::unordered_map<std::string, std::string> op_map_;
};
