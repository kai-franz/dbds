
#include "SimulatedOptimization.h"
#include "llvm/IR/Constant.h"
#include "llvm/Analysis/ConstantFolding.h"

namespace llvm {

bool simulateCF(SimulationResult &result) {
  outs() << "Simulating ConstantFolding for block ";
  result.BB->printAsOperand(outs());
  outs() << " with parent ";
  result.predBB->printAsOperand(outs());
  outs() << "\n";
  bool changed = false;
  for (auto BB_it = result.BB->begin(), BB_end = result.BB->end(); BB_it != BB_end; ++BB_it) {
    Instruction &I = *BB_it;
    // iterate over operands of I
    bool all_constants = true;
    for (auto op_it = I.op_begin(), op_end = I.op_end(); op_it != op_end; ++op_it) {
      // if operand is a constant
      if (!isa<Constant>(result.lookupInst(*op_it))) {
        outs() << "Operand is not a constant: " << *result.lookupInst(*op_it) << "\n";
        all_constants = false;
        break;
      }
    }
    if (all_constants) {
      // if all operands are constants
      auto *new_I = I.clone();
      result.replaceOperands(new_I);
      Constant *C = ConstantFoldInstruction(new_I, I.getModule()->getDataLayout());
      if (C) {
        outs() << "Found constant folding for " << I << " to " << *C << "\n";
        result.synonymMap[&I] = C;
        changed = true;
      }
    }
  }
  return changed;
}

}
