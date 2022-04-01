
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include "SimulationResult.h"
#include "SimulatedOptimization.h"
#include "ConstantFolding.h"

using namespace llvm;

namespace {

class DbdsPass : public FunctionPass {
 public:
  static char ID;
  std::vector<SimulatedOptimization> optimizations { simulateCF };
  std::vector<SimulationResult> opts;

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
            simulate(BB, *pred_it);
          }
        }
        ++WorkStack.back().second;
        WorkStack.push_back({Child, Child->begin()});
      }
    }
    return modified;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<DominatorTreeWrapperPass>();
  }

  void simulate(BasicBlock *BB, BasicBlock *predBB) {
    // ValueMap of phi instructions to their respective values
    SimulationResult result(BB, predBB);
    for (auto &phi : BB->phis()) {
      result.synonymMap[&phi] = phi.getIncomingValueForBlock(predBB);
      outs() << "incoming value: " << *phi.getIncomingValueForBlock(predBB) << "\n";
    }
    // simulate all optimizations
    for (auto &opt : optimizations) {
      opt(result);
    }
    opts.push_back(result);
  }

};

char DbdsPass::ID = 0;
RegisterPass<DbdsPass> X("dbds", "15745 Dominance-Based Deduplication Simulation");


}

