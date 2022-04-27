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

typedef ValueMap<Value *, std::pair<Instruction *, WeakTrackingVH>> SynonymMap;

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

  Value *lookupWithGlobalMap(Value *V) const {
    if (synonymMap->find(V) != synonymMap->end()) {
      Value *local = (*synonymMap)[V].second;
      if (globalSynonymMap->find(local) != globalSynonymMap->end() &&
          isa<Instruction>(local) &&
          dyn_cast<Instruction>(local)->getParent() != BB
          ) {
        return (*globalSynonymMap)[local].second;
      }
      return local;
    }
    if (globalSynonymMap->find(V) != globalSynonymMap->end() &&
        isa<Instruction>(V) &&
        dyn_cast<Instruction>(V)->getParent() != BB
        ) {
      return (*globalSynonymMap)[V].second;
    }

    return nullptr;
  }

  // Looks up the replacement for a value as an operand.
 Value *lookup(Value *V) {
   if (synonymMap->find(V) != synonymMap->end()) {
     return (*synonymMap)[V].second;
   }
   return nullptr;
 }

 // Looks up the replacement for a value itself.
 Instruction *lookupReplacement(Value *V) {
   if (synonymMap->find(V) != synonymMap->end()) {
     return (*synonymMap)[V].first;
   }
   return nullptr;
 }

 // Sets the replacement for a value.
 // Implicitly sets the replacement for the instruction itself to nullptr.
 void set(Value *key, Value *value) {
   (*synonymMap)[key] = std::make_pair(nullptr, value);
 }

 // Sets the replacement for a value and the instruction itself.
 void set(Value *key, Value *value, Instruction *inst) {
   (*synonymMap)[key] = std::make_pair(inst, value);
 }

 bool hasReplacement(Value *key) {
   return synonymMap->find(key) != synonymMap->end();
 }

};

}
#endif //DBDS__SIMULATIONRESULT_H_
