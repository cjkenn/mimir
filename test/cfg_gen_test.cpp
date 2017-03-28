#include <assert.h>
#include <vector>
#include <memory>
#include <iostream>
#include "../src/ir_instr.h"
#include "../src/cfg_gen.h"
#include "../src/cfg.h"
#include "../src/cfg_node.h"

// TODO: Maybe should go into a test_helper file
std::vector<IrInstrPtr> build_if_ir() {
  std::vector<IrInstrPtr> ir;

  auto ir1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR,"x","r0","r0");
  ir1->SetLabel("lbl0");
  ir.push_back(ir1);

  auto ir2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR,"1","r1","r1");
  ir2->SetLabel("lbl0");
  ir.push_back(ir2);

  auto ir3 = std::make_shared<IrInstr>(IrInstrType::CMP_INSTR, "r0", "r1", "r1");
  ir3->SetLabel("lbl0");
  ir.push_back(ir3);

  auto ir4 = std::make_shared<IrInstr>(IrInstrType::JMPGT_INSTR, "lbl1");
  ir4->SetLabel("lbl0");
  ir.push_back(ir4);

  auto ir5 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR, "2", "r2", "r2");
  ir5->SetLabel("lbl0");
  ir.push_back(ir5);

  auto ir6 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r2");
  ir6->SetLabel("lbl0");
  ir.push_back(ir6);

  auto ir7 = std::make_shared<IrInstr>(IrInstrType::NOP_INSTR, "xx");
  ir7->SetLabel("lbl1");
  ir.push_back(ir7);

  return ir;
}

std::vector<IrInstrPtr> build_while_ir() {
  std::vector<IrInstrPtr> ir;

  auto ir1 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR,"x", "r0", "r0");
  ir1->SetLabel("lbl0");
  ir.push_back(ir1);

  auto ir2 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR,"10", "r1", "r1");
  ir2->SetLabel("lbl0");
  ir.push_back(ir2);

  auto ir3 = std::make_shared<IrInstr>(IrInstrType::CMP_INSTR, "r0", "r1", "r1");
  ir3->SetLabel("lbl0");
  ir.push_back(ir3);

  auto ir4 = std::make_shared<IrInstr>(IrInstrType::JMPGT_INSTR, "lbl1");
  ir4->SetLabel("lbl0");
  ir.push_back(ir4);

  auto ir5 = std::make_shared<IrInstr>(IrInstrType::LD_INSTR, "x", "r2", "r2");
  ir5->SetLabel("lbl0");
  ir.push_back(ir5);

  auto ir6 = std::make_shared<IrInstr>(IrInstrType::MV_INSTR,"1", "r3", "r3");
  ir6->SetLabel("lbl0");
  ir.push_back(ir6);

  auto ir7 = std::make_shared<IrInstr>(IrInstrType::ADD_INSTR, "r2", "r3", "r3");
  ir7->SetLabel("lbl0");
  ir.push_back(ir7);

  auto ir8 = std::make_shared<IrInstr>(IrInstrType::SV_INSTR, "r3");
  ir8->SetLabel("lbl0");
  ir.push_back(ir8);

  auto ir9 = std::make_shared<IrInstr>(IrInstrType::JMP_INSTR, "lbl0");
  ir9->SetLabel("lbl0");
  ir.push_back(ir9);

  auto ir10 = std::make_shared<IrInstr>(IrInstrType::NOP_INSTR, "xx");
  ir10->SetLabel("lbl1");
  ir.push_back(ir10);

  return ir;
}

// Input:
//
// lbl0:
// ld x, r0
// mv 1, r1
// cmp r0, r1
// jmpgt lbl1
// mv 2, r2
// sv r2
// lbl1:
//
// Expected Output:
//
//    n0 -> n1
void test_gen_if_ir() {
  auto ir = build_if_ir();
  CfgGen cfg_gen;
  auto root = cfg_gen.Gen(ir).GetRoot();

  assert(root->GetAdj().size() == 1);
  assert(root->GetInstrs().size() == 6);

  auto root_instrs = root->GetInstrs();
  assert(root_instrs[0]->GetType() == IrInstrType::LD_INSTR);
  assert(root_instrs[1]->GetType() == IrInstrType::MV_INSTR);
  assert(root_instrs[2]->GetType() == IrInstrType::CMP_INSTR);
  assert(root_instrs[3]->GetType() == IrInstrType::JMPGT_INSTR);
  assert(root_instrs[4]->GetType() == IrInstrType::MV_INSTR);
  assert(root_instrs[5]->GetType() == IrInstrType::SV_INSTR);

  auto second = root->GetAdj()[0];
  assert(second->GetAdj().size() == 1); // has a dummy exit child
  assert(second->GetInstrs().size() == 1);

  auto second_instrs = second->GetInstrs();
  assert(second_instrs[0]->GetType() == IrInstrType::NOP_INSTR);
  assert(second_instrs[0]->GetLabel() == "lbl1");
}

// Input:
//
// lbl0:
// ld x, r0
// mv 10, r1
// cmp r0, r1
// jmpgt lbl1
// ld x, r2
// mv 1, r3
// add r2, r3
// sv r3
// jmp lbl0
// lbl1:
//
// Expected Output:
//
//   n0 -> n0 -> n1
// graph has a cycle from n0 -> n0
void test_gen_while_ir() {
  auto ir = build_while_ir();
  CfgGen cfg_gen;

  auto root = cfg_gen.Gen(ir).GetRoot();

  assert(root->GetAdj().size() == 2);
  assert(root->GetInstrs().size() == 9);

  auto root_instrs = root->GetInstrs();
  assert(root_instrs[0]->GetType() == IrInstrType::LD_INSTR);
  assert(root_instrs[1]->GetType() == IrInstrType::MV_INSTR);
  assert(root_instrs[2]->GetType() == IrInstrType::CMP_INSTR);
  assert(root_instrs[3]->GetType() == IrInstrType::JMPGT_INSTR);
  assert(root_instrs[4]->GetType() == IrInstrType::LD_INSTR);
  assert(root_instrs[5]->GetType() == IrInstrType::MV_INSTR);
  assert(root_instrs[6]->GetType() == IrInstrType::ADD_INSTR);
  assert(root_instrs[7]->GetType() == IrInstrType::SV_INSTR);
  assert(root_instrs[8]->GetType() == IrInstrType::JMP_INSTR);

  auto next = root->GetAdj()[0];
  assert(next->GetAdj().size() == 1); // has a dummy exit child
  assert(next->GetInstrs().size() == 1);

  auto next_instrs = next->GetInstrs();
  assert(next_instrs[0]->GetType() == IrInstrType::NOP_INSTR);
  assert(next_instrs[0]->GetLabel() == "lbl1");

  auto cycle = root->GetAdj()[1];
  assert(cycle->GetAdj().size() == 2);
  assert(cycle->GetName() == "n0");
}

int main(int argc, char **argv) {
  test_gen_if_ir();
  test_gen_while_ir();

  std::cout << "Cfg gen tests passed!" << std::endl;
  return 0;
}
