#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>
#include "local_optimizer.h"
#include "cfg_node.h"
#include "ir_instr.h"
#include "helper/local_optimizer.h"

// Expected input instructions:
// ld a, r0
// ld d, r1
// sub r0, r1
// sv r1, b
// ld b, r2
// ld c, r3
// add r2, r3
// sv r3, c
// ld a, r4
// ld d, r5
// sub r4, r5
// sv r5, d
//
// Expected output instructions:
// ld a, r0
// ld d, r1
// sub r0, r1
// sv r1, b
// ld b, r2
// ld c, r3
// add r2, r3
// sv r3, c
// sv b, d
void test_opt_for_redundancy(LocalOptimizerTestHelper helper) {
  auto block = helper.GetRedundantBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 12);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 9);
  auto last_instr = block->GetInstrs().back();

  assert(last_instr->GetType() == IrInstrType::SV_INSTR);
  assert(last_instr->GetArgs().first == "b");
  assert(last_instr->GetArgs().second == "d");
  assert(last_instr->GetDest() == "d");

  // assert that the second last instruction is a sv as well, to ensure
  // that the ld instructions were removed
  auto second_last_instr = block->GetInstrs()[7];

  assert(second_last_instr->GetType() == IrInstrType::SV_INSTR);
  assert(second_last_instr->GetArgs().first == "r3");
  assert(second_last_instr->GetArgs().second == "c");
  assert(second_last_instr->GetDest() == "c");
}

// Expected input instructions:
// ld a, r0
// ld d, r1
// add r0, r1
// sv r1, a
// ld d, r2
// ld a, r3
// add r2, r3
// sv r3, b
//
// Expected output instructions:
// ld a, r0
// ld d, r1
// add r0, r1
// sv r1, a
// sv a, b
void test_opt_for_commutivity(LocalOptimizerTestHelper helper) {
  auto block = helper.GetCommutativeBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 8);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 5);
  auto last_instr = block->GetInstrs().back();

  assert(last_instr->GetType() == IrInstrType::SV_INSTR);
  assert(last_instr->GetArgs().first == "a");
  assert(last_instr->GetArgs().second == "b");
  assert(last_instr->GetDest() == "b");

  // assert that the second last instruction is a sv as well, to ensure
  // that the ld instructions were removed
  auto second_last_instr = block->GetInstrs()[3];

  assert(second_last_instr->GetType() == IrInstrType::SV_INSTR);
  assert(second_last_instr->GetArgs().first == "r1");
  assert(second_last_instr->GetArgs().second == "a");
  assert(second_last_instr->GetDest() == "a");
}


// Expected input instructions:
// ld a, r0
// ld d, r1
// sub r0, r1
// sv r1, a
// ld d, r2
// ld a, r3
// sub r2, r3
// sv r3, b
//
// Expected output is the same, there is nothing to optimize here!
// This test is intended to make sure that non-commutative operations
// aren't accidentally optimized.
void test_opt_for_non_commutivity(LocalOptimizerTestHelper helper) {
  auto block = helper.GetNonCommutativeBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 8);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 8);
}

// Expected input instructions:
// mv 2, r0
// mv 3, r1
// add r0, r1
// sv r1, b
//
// Expected output instructions:
// mv 5, r1
// sv r1, b
void test_opt_constant_folding(LocalOptimizerTestHelper helper) {
  auto block = helper.GetConstFoldBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 4);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 2);
  auto instr = block->GetInstrs()[0];

  assert(instr->GetType() == IrInstrType::MV_INSTR);
  assert(instr->GetArgs().first == "5");
  assert(instr->GetArgs().second == "r1");
  assert(instr->GetDest() == "r1");
}

// Expected input instructions:
// mv 0, r0
// ld a, r1
// add r0, r1
//
// Expected output instructions:
// ld a, r1
void test_opt_add_id(LocalOptimizerTestHelper helper) {
  auto block = helper.GetAddIdBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 3);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 1);
  auto instr = block->GetInstrs()[0];

  assert(instr->GetType() == IrInstrType::LD_INSTR);
  assert(instr->GetArgs().first == "a");
  assert(instr->GetArgs().second == "r1");
  assert(instr->GetDest() == "r1");
}

// Expected input instructions:
// ld a, r0
// mv 0, r1
// sub r0, r1
//
// Expected output instructions:
// ld a, r1
void test_opt_sub_by_zero(LocalOptimizerTestHelper helper) {
  auto block = helper.GetSubByZeroBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 3);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 1);
  auto instr = block->GetInstrs()[0];

  assert(instr->GetType() == IrInstrType::LD_INSTR);
  assert(instr->GetArgs().first == "a");
  assert(instr->GetArgs().second == "r1");
  assert(instr->GetDest() == "r1");
}

// Expected input instructions:
// ld a, r0
// ld a, r1
// sub r0, r1
//
// Expected output instructions:
// mv 0, r1
void test_opt_sub_by_self(LocalOptimizerTestHelper helper) {
  auto block = helper.GetSubBySelfBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 3);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 1);
  auto instr = block->GetInstrs()[0];

  assert(instr->GetType() == IrInstrType::MV_INSTR);
  assert(instr->GetArgs().first == "0");
  assert(instr->GetArgs().second == "r1");
  assert(instr->GetDest() == "r1");
}

// Expected input instructions:
// ld a, r0
// mv 2, r1
// mul r0, r1
//
// Expected output instructions:
// ld a, r0
// ld a, r1
// add r0, r1
void test_opt_mul_by_two(LocalOptimizerTestHelper helper) {
  auto block = helper.GetMulByTwoBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 3);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 3);
  auto instr1 = block->GetInstrs()[0];

  assert(instr1->GetType() == IrInstrType::LD_INSTR);
  assert(instr1->GetArgs().first == "a");
  assert(instr1->GetArgs().second == "r0");
  assert(instr1->GetDest() == "r0");

  auto instr2 = block->GetInstrs()[1];

  assert(instr2->GetType() == IrInstrType::LD_INSTR);
  assert(instr2->GetArgs().first == "a");
  assert(instr2->GetArgs().second == "r1");
  assert(instr2->GetDest() == "r1");

  auto instr3 = block->GetInstrs()[2];

  assert(instr3->GetType() == IrInstrType::ADD_INSTR);
  assert(instr3->GetArgs().first == "r0");
  assert(instr3->GetArgs().second == "r1");
  assert(instr3->GetDest() == "r1");
}

// Expected input instructions:
// ld a, r0
// mv 1, r1
// mul r0, r1
//
// Expected output instructions:
// ld a, r1
void test_opt_mul_by_one(LocalOptimizerTestHelper helper) {
  auto block = helper.GetMulByOneBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 3);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 1);
  auto instr = block->GetInstrs()[0];

  assert(instr->GetType() == IrInstrType::LD_INSTR);
  assert(instr->GetArgs().first == "a");
  assert(instr->GetArgs().second == "r1");
  assert(instr->GetDest() == "r1");
}

// Expected input instructions:
// ld a, r0
// mv 0, r1
// mul r0, r1
//
// Expected output instructions:
// mv 0, r1
void test_opt_mul_by_zero(LocalOptimizerTestHelper helper) {
  auto block = helper.GetMulByZeroBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 3);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 1);
  auto instr = block->GetInstrs()[0];

  assert(instr->GetType() == IrInstrType::MV_INSTR);
  assert(instr->GetArgs().first == "0");
  assert(instr->GetArgs().second == "r1");
  assert(instr->GetDest() == "r1");
}

// Expected input instructions:
// ld a, r0
// mv 1, r1
// div r0, r1
//
// Expected output instructions:
// ld a, r1
void test_opt_div_by_one(LocalOptimizerTestHelper helper) {
  auto block = helper.GetDivByOneBlock();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 3);

  lo.OptimizeBlock(block);

  assert(block->GetInstrs().size() == 1);
  auto instr = block->GetInstrs()[0];

  assert(instr->GetType() == IrInstrType::LD_INSTR);
  assert(instr->GetArgs().first == "a");
  assert(instr->GetArgs().second == "r1");
  assert(instr->GetDest() == "r1");
}

int main(int argc, char **argv) {
  LocalOptimizerTestHelper helper;
  test_opt_for_redundancy(helper);

  test_opt_for_commutivity(helper);
  test_opt_for_non_commutivity(helper);

  test_opt_constant_folding(helper);

  test_opt_add_id(helper);

  test_opt_sub_by_self(helper);
  test_opt_sub_by_zero(helper);

  test_opt_mul_by_two(helper);
  test_opt_mul_by_one(helper);
  test_opt_mul_by_zero(helper);

  test_opt_div_by_one(helper);

  std::cout << "Local optimization tests passed!" << std::endl;

  return 0;
}
