//
// Created by Kai Franz on 3/31/22.
//

#ifndef DBDS__SIMULATIONRESULT_H_
#define DBDS__SIMULATIONRESULT_H_

#include "llvm/IR/Value.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/Instruction.h"
#include <unordered_map>

namespace llvm {

typedef std::unordered_map<Value *, Value *> SynonymMap;

struct SimulationResult {
 BasicBlock *BB;
 BasicBlock *predBB;
 SynonymMap synonymMap;

 SimulationResult(BasicBlock *BB, BasicBlock *predBB) : BB(BB), predBB(predBB) {}

 Value *lookupInst(Value *V) {
  Value *synonym = synonymMap[V];
  if (synonym)
   return synonym;
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
