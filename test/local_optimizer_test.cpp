#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../src/local_optimizer.h"
#include "../src/cfg_node.h"
#include "../src/ir_instr.h"

CfgNodePtr get_cfg_block_for_lvn() {
  // ALl we care about is the instruction vecotr here, so we
  // don't bother setting any of the other cfg properties.
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "d", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::SUB_INSTR, "r0", "r1", "r1");
  auto instr4 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r1", "b", "b");

  auto instr5 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "b", "r2", "r2");
  auto instr6 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "c", "r3", "r3");
  auto instr7 = std::make_shared<IrInstr>(IrInstrType::ADD_INSTR, "r2", "r3", "r3");
  auto instr8 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r3", "c", "c");

  auto instr9 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r4", "r4");
  auto instr10 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "d", "r5", "r5");
  auto instr11 = std::make_shared<IrInstr>(IrInstrType::SUB_INSTR, "r4", "r5", "r5");
  auto instr12 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r5", "d", "d");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  instrs.push_back(instr4);
  instrs.push_back(instr5);
  instrs.push_back(instr6);
  instrs.push_back(instr7);
  instrs.push_back(instr8);
  instrs.push_back(instr9);
  instrs.push_back(instr10);
  instrs.push_back(instr11);
  instrs.push_back(instr12);

  block->SetInstrs(instrs);

  return block;
}

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
void test_lvn() {
  auto block = get_cfg_block_for_lvn();
  LocalOptimizer lo;

  assert(block->GetInstrs().size() == 12);

  lo.Lvn(block);

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

int main(int argc, char **argv) {
  test_lvn();

  std::cout << "Lvn tests passed!" << std::endl;

  return 0;
}
