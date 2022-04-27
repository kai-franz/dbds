//
// Created by Kai Franz on 4/26/22.
//

#ifndef DBDS__READELIMINATION_H_
#define DBDS__READELIMINATION_H_


#include "SimulatedOptimization.h"
#include "llvm/IR/Constant.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "available-support.h"

namespace llvm {

bool simulateRE(SimulationResult &result) {
  outs() << "Simulating ReadElimination for block ";
  result.BB->printAsOperand(outs());
  outs() << " with parent ";
  result.predBB->printAsOperand(outs());
  outs() << "\n";
  bool changed = false;
  for (auto BB_it = result.BB->begin(), BB_end = result.BB->end(); BB_it != BB_end; ++BB_it) {
    Instruction &I = *BB_it;
    // check if instruction is a binary operation
    if (auto *BI = dyn_cast<BinaryOperator>(&I)) {
      Expression expr ()
    }
  }
  return changed;
}

}


#endif //DBDS__READELIMINATION_H_
