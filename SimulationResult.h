//
// Created by Kai Franz on 3/31/22.
//

#ifndef DBDS__SIMULATIONRESULT_H_
#define DBDS__SIMULATIONRESULT_H_

#include "llvm/IR/Value.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/ValueHandle.h"
#include <unordered_map>

namespace llvm {

typedef ValueMap<Value *, TrackingVH<Value>> SynonymMap;

struct SimulationResult {
 BasicBlock *BB;
 BasicBlock *predBB;
 std::unique_ptr<SynonymMap> _synonymMap;
 SynonymMap &synonymMap;

 SimulationResult(BasicBlock *BB, BasicBlock *predBB) : BB(BB), predBB(predBB), _synonymMap(new SynonymMap()), synonymMap(*_synonymMap) {}

 Value *lookupInst(Value *V) {
   auto it = synonymMap.find(V);
   if (it != synonymMap.end()) {
     return it->second;
   }
   return V;
 }
 void replaceOperands(Instruction *I) {
  for (unsigned i = 0; i < I->getNumOperands(); i++) {
   Value *V = I->getOperand(i);
   I->setOperand(i, lookupInst(V));
  }
 }
};

}
#endif //DBDS__SIMULATIONRESULT_H_
