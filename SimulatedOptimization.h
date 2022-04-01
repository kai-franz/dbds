#ifndef DBDS__SIMULATEDOPTIMIZATION_H_
#define DBDS__SIMULATEDOPTIMIZATION_H_

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include "SimulationResult.h"

namespace llvm {

typedef std::function <bool (SimulationResult&)> SimulatedOptimization;


}

#endif //DBDS__SIMULATEDOPTIMIZATION_H_
