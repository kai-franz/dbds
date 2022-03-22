#ifndef DBDS__SIMULATEDOPTIMIZATION_H_
#define DBDS__SIMULATEDOPTIMIZATION_H_

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"

namespace llvm {

class SimulatedOptimization {
 public:
  virtual void simulate(BasicBlock *BB, ValueMap<Value *, Value *> phi_vals) = 0;
};

}

#endif //DBDS__SIMULATEDOPTIMIZATION_H_
