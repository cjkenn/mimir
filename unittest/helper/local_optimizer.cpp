#include "local_optimizer.h"

using namespace mimir;

CfgNodePtr LocalOptimizerTestHelper::GetRedundantBlock() {
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

CfgNodePtr LocalOptimizerTestHelper::GetCommutativeBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "d", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::ADD_INSTR, "r0", "r1", "r1");
  auto instr4 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r1", "a", "a");

  auto instr5 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "d", "r2", "r2");
  auto instr6 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r3", "r3");
  auto instr7 = std::make_shared<IrInstr>(IrInstrType::ADD_INSTR, "r2", "r3", "r3");
  auto instr8 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r3", "b", "b");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  instrs.push_back(instr4);
  instrs.push_back(instr5);
  instrs.push_back(instr6);
  instrs.push_back(instr7);
  instrs.push_back(instr8);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetNonCommutativeBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "d", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::SUB_INSTR, "r0", "r1", "r1");
  auto instr4 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r1", "a", "a");

  auto instr5 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "d", "r2", "r2");
  auto instr6 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r3", "r3");
  auto instr7 = std::make_shared<IrInstr>(IrInstrType::SUB_INSTR, "r2", "r3", "r3");
  auto instr8 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r3", "b", "b");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  instrs.push_back(instr4);
  instrs.push_back(instr5);
  instrs.push_back(instr6);
  instrs.push_back(instr7);
  instrs.push_back(instr8);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetConstFoldBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "2", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "3", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::ADD_INSTR, "r0", "r1", "r1");
  auto instr4 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r1", "b", "b");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  instrs.push_back(instr4);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetAddIdBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "0", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::ADD_INSTR, "r0", "r1", "r1");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetSubByZeroBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "0", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::SUB_INSTR, "r0", "r1", "r1");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetSubBySelfBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::SUB_INSTR, "r0", "r1", "r1");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetMulByTwoBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "2", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::MUL_INSTR, "r0", "r1", "r1");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetMulByOneBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "1", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::MUL_INSTR, "r0", "r1", "r1");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetMulByZeroBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "0", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::MUL_INSTR, "r0", "r1", "r1");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  block->SetInstrs(instrs);

  return block;
}

CfgNodePtr LocalOptimizerTestHelper::GetDivByOneBlock() {
  CfgNodePtr block = std::make_shared<CfgNode>("n0");

  std::vector<IrInstrPtr> instrs;
  auto instr1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "a", "r0", "r0");
  auto instr2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "1", "r1", "r1");
  auto instr3 = std::make_shared<IrInstr>(IrInstrType::DIV_INSTR, "r0", "r1", "r1");

  instrs.push_back(instr1);
  instrs.push_back(instr2);
  instrs.push_back(instr3);
  block->SetInstrs(instrs);

  return block;
}
