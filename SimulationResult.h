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
 std::unique_ptr<SynonymMap> synonymMap;
 SynonymMap *globalSynonymMap;

 SimulationResult(BasicBlock *BB, BasicBlock *predBB, SynonymMap *global) :
  BB(BB), predBB(predBB), synonymMap(new SynonymMap()), globalSynonymMap(global) {}

 Value *lookupInst(Value *V) {
   auto syn = lookup(V);
   if (syn != nullptr) {
     return syn;
   }
   return V;
 }
 void replaceOperands(Instruction *I) {
  for (unsigned i = 0; i < I->getNumOperands(); i++) {
   Value *V = I->getOperand(i);
   I->setOperand(i, lookupInst(V));
  }
 }

//  Value *lookup(Value *V) const {
//    if (globalSynonymMap->find(V) != globalSynonymMap->end() &&
//        isa<Instruction>((*globalSynonymMap)[V].getValPtr()) &&
//        isa<Instruction>(V) &&
//        dyn_cast<Instruction>((*globalSynonymMap)[V].getValPtr())->getParent() !=
////                 dyn_cast<Instruction>(V)->getParent()
//            predBB
//        ) {
//      return (*globalSynonymMap)[V];
//    }
//    if (synonymMap->find(V) != synonymMap->end()) {
//      return (*synonymMap)[V];
//    }
//    return nullptr;
//  }

 Value *lookup(Value *V) {
   if (globalSynonymMap->find(V) != globalSynonymMap->end()) {
     return (*globalSynonymMap)[V];
   }
   if (synonymMap->find(V) != synonymMap->end()) {
     return (*synonymMap)[V];
   }
   return nullptr;
 }

 void set(Value *key, Value *value) {
   (*synonymMap)[key] = value;
 }

};

}
#endif //DBDS__SIMULATIONRESULT_H_
