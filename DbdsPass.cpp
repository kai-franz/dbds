
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include "SimulationResult.h"
#include "SimulatedOptimization.h"
#include "ConstantFolding.h"
#include "StrengthReduction.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "ReadElimination.h"


using namespace llvm;

namespace {

class DbdsPass : public FunctionPass {
 public:
  static char ID;
  std::vector<SimulatedOptimization> optimizations { simulateCF, simulateSR, simulateRE };
  std::vector<SimulationResult> opts;
  SynonymMap globalMap;
  std::vector<BasicBlock*> toDelete;

  DbdsPass() : FunctionPass(ID) {}

   bool runOnFunction(Function &F) override {
    DominatorTree &D = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
    bool modified = false;

    // depth-first traversal based on implementation of DominatorTreeBase< NodeT, IsPostDom >::updateDFSNumbers()
    // from GenericDomTree.h
    SmallVector<std::pair<const DomTreeNodeBase<DominatorTree::NodeType> *,
                          typename DomTreeNodeBase<DominatorTree::NodeType>::const_iterator>,
                32> WorkStack;

    const DomTreeNodeBase<DominatorTree::NodeType> *ThisRoot = D.getRootNode();
    if (!ThisRoot) {
      errs() << "No root node found\n";
      return false;
    }

    WorkStack.push_back({ThisRoot, ThisRoot->begin()});

    while (!WorkStack.empty()) {
      const DomTreeNodeBase<DominatorTree::NodeType> *Node = WorkStack.back().first;
      const auto ChildIt = WorkStack.back().second;

      if (ChildIt == Node->end()) {
        WorkStack.pop_back();
      } else {
        const DomTreeNodeBase<DominatorTree::NodeType> *Child = *ChildIt;
        BasicBlock *BB = Child->getBlock();
        BB->printAsOperand(outs(), false);
        outs() << "\n";
        if (BB->getSinglePredecessor() == nullptr) {
          // iterate over BB preds
          for (auto pred_it = pred_begin(BB), p_e = pred_end(BB); pred_it != p_e; ++pred_it) {
            // check if *pred_it has multiple successors
            if ((*pred_it)->getSingleSuccessor() == BB) {
              simulate(BB, *pred_it);
            }
          }
        }
        // print instructions in BB
        outs() << *BB << "\n";
        ++WorkStack.back().second;
        WorkStack.push_back({Child, Child->begin()});
      }
    }
    applySimulationResults();


   for(auto &BB : toDelete) {
     outs() << "deleting ";
     BB->printAsOperand(outs(), false);
     outs() << "\n";
     DeleteDeadBlock(BB);
   }

     outs() << "--------- final BBs ---------\n";

     for(auto &BB : F) {
       outs() << BB << "\n";
     }

    while (mergeBlocks(F)) {
      modified = true;
    }
    return modified;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<DominatorTreeWrapperPass>();
  }

  void simulate(BasicBlock *BB, BasicBlock *predBB) {
    // ValueMap of phi instructions to their respective values
    opts.emplace_back(BB, predBB, &globalMap);
    auto &result = opts.back();
    for (auto &phi : BB->phis()) {
      result.set(&phi, phi.getIncomingValueForBlock(predBB));
      outs() << "incoming value: " << *phi.getIncomingValueForBlock(predBB) << "\n";
    }
    // simulate all optimizations
    for (auto &opt : optimizations) {
      opt(result);
    }
  }

  void applySimulationResults() {
    for (auto &opt : opts) {
    outs() << "-----------------------------------Applying simulation results\n";
      // print synonym map
      for (const auto &synonym : *opt.synonymMap) {
        if (synonym.first != nullptr && synonym.second.second != nullptr) {
          outs() << "synonym: " << *synonym.first << " -> " << *synonym.second.second << "\n";
        }
      }
      opt.predBB->getTerminator()->eraseFromParent();
      BasicBlock *phiBB = opt.BB->splitBasicBlock(opt.BB->getTerminator(), "split");
      BranchInst::Create(phiBB, opt.predBB);
      // split opt.BB right before its terminator
      for (auto &inst : *opt.BB) {
        if (&inst == opt.BB->getTerminator()) {
          break;
        }
        Value *replacementValue;
        if (auto phi = dyn_cast<PHINode>(&inst)) {
          outs() << "phi: " << *phi << "\n";
          phi->removeIncomingValue(opt.predBB, false);
          replacementValue = opt.lookup(&inst);
        }
        else {
          Instruction *replacementInst = opt.lookupReplacement(&inst);
          replacementValue = opt.lookup(&inst);
          outs() << "inst: " << inst << "\n";
          if (!opt.hasReplacement(&inst)) {
            replacementInst = inst.clone();
            replacementInst->setName(inst.getName() + ".sim");
            replacementValue = replacementInst;
          } else if (replacementInst != nullptr) {
            outs() << "replacing " << inst << " with " << *replacementValue << "\n";
          } else {
            outs() << "deleting " << inst << "\n";
          }
          if (replacementInst != nullptr) {
            // iterator over replacementInst's operands
            for (auto OI = replacementInst->op_begin(), OE = replacementInst->op_end(); OI != OE; ++OI) {
              if (isa<Instruction>(*OI)) {
                auto syn = opt.lookupWithGlobalMap(dyn_cast<Instruction>(*OI));
                if (syn != nullptr && syn != &*OI->getUser()) {
                  outs() << "replacing " << **OI << " in " << (*OI->getUser()) << " with " << *syn << "\n";
                  *OI = syn;
                }
              }
            }
            opt.set(&inst, replacementInst);
            replacementInst->insertBefore(opt.predBB->getTerminator());
            outs() << "adding " << *replacementInst << "\n";
          }
        }
        // iterate over the inst's uses, checking if there is at least one oustide of BB
        bool usedOutside = false;
        for (auto UI = inst.use_begin(), UE = inst.use_end(); UI != UE; ++UI) {
          auto *I = dyn_cast<Instruction>(UI->getUser());
          if (I != nullptr && (I->getParent() != opt.BB || isa<PHINode>(I))) {
            if (isa<PHINode>(I)) {
              outs() << *I << " is a phi node" << "\n";
            }
            if (I->getParent() != opt.BB) {
              outs() << *I << " parent is not original BB: ";
              I->getParent()->printAsOperand(outs(), false);
              outs() << "\n";
            }
            usedOutside = true;
            break;
          }
        }
        if (usedOutside) {
          // add a phi node to phiBB with arguments for replacementInst and inst
          PHINode *phi = PHINode::Create(replacementValue->getType(), 2, "");
          phi->insertBefore(phiBB->getTerminator());
          phi->addIncoming(replacementValue, opt.predBB);
          phi->addIncoming(&inst, opt.BB);
          outs() << "added phi node: " << *phi << "\n";
          inst.replaceUsesWithIf(phi, [&](Use &U) {
            bool phiNode = isa<PHINode>(U.getUser());
            auto parent = dyn_cast<Instruction>(U.getUser())->getParent();
            // Replace all uses that see the phi node.
            // We don't replace the following:
            // 1. The other uses inside the original basic block
            // 2. The uses inside the phi node we just added
            // Every other use is replaced with the phi node we created.
            // Other uses inside the original basic block cannot use the phi node
            // because of self-loops.
            return (phiNode || parent != opt.BB) && !(parent == phiBB && phiNode);
          });
          if (globalMap.find(&inst) == globalMap.end()) {
            globalMap[&inst] = std::make_pair(nullptr, phi);
          }
        }
      }
      // if opt.BB has no predecessors, remove it
      if (!opt.BB->hasNPredecessorsOrMore(1)) {
        toDelete.push_back(opt.BB);
      }
    }
  }

  bool mergeBlocks(Function &F) {
    for (auto &BB : F) {
      if (MergeBlockIntoPredecessor(&BB)) {
        return true;
      }
    }
    return false;
  }
};

char DbdsPass::ID = 0;
RegisterPass<DbdsPass> X("dbds", "15745 Dominance-Based Deduplication Simulation");

} // namespace

