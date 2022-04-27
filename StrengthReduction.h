
#ifndef DBDS__STRENGTHREDUCTION_H_
#define DBDS__STRENGTHREDUCTION_H_


#include "SimulatedOptimization.h"
#include "llvm/IR/Constant.h"
#include "llvm/Analysis/ConstantFolding.h"

namespace llvm {

bool simulateSR(SimulationResult &result) {
  outs() << "Simulating StrengthReduction for block ";
  result.BB->printAsOperand(outs());
  outs() << " with parent ";
  result.predBB->printAsOperand(outs());
  outs() << "\n";
  bool changed = false;
  for (auto BB_it = result.BB->begin(), BB_end = result.BB->end(); BB_it != BB_end; ++BB_it) {
    Instruction &I = *BB_it;
    // check if instruction is a binary operation
    if (auto *BI = dyn_cast<BinaryOperator>(&I)) {
      if (BI->getOpcode() == Instruction::Mul) {
        auto *Op0 = dyn_cast<ConstantInt>(result.lookupInst(BI->getOperand(0)));
        auto *Op1 = dyn_cast<ConstantInt>(result.lookupInst(BI->getOperand(1)));
        if (Op0 != nullptr && Op0->getValue().isPowerOf2()) {
          outs() << "Operand 0 is power of 2\n";
          auto C = ConstantInt::get(Op0->getType(), Op0->getValue().exactLogBase2());
          auto new_inst = BinaryOperator::Create(Instruction::Shl, BI->getOperand(1), C);
          result.set(&I, new_inst, new_inst);
          changed = true;
        } else if (Op1 != nullptr && Op1->getValue().isPowerOf2()) {
          outs() << "Operand 1 is power of 2\n";
          auto C = ConstantInt::get(Op1->getType(), Op1->getValue().exactLogBase2());
          auto new_inst = BinaryOperator::Create(Instruction::Shl, BI->getOperand(0), C);
          result.set(&I, new_inst, new_inst);
          changed = true;
        }
      } else if (BI->getOpcode() == Instruction::UDiv) {
        auto *Op1 = dyn_cast<ConstantInt>(result.lookupInst(BI->getOperand(1)));
//        if ()
//        } else
          if (Op1 != nullptr && Op1->getValue().isPowerOf2()) {
          auto C = ConstantInt::get(Op1->getType(), Op1->getValue().exactLogBase2());
          auto new_inst = BinaryOperator::Create(Instruction::LShr, BI->getOperand(0), C);
          result.set(&I, new_inst, new_inst);
          changed = true;
        }
      }
    }
  }
  return changed;
}

}


#endif //DBDS__STRENGTHREDUCTION_H_
