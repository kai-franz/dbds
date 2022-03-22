# Implementation of Dominance-Based Duplication Simulation in LLVM
Kai Franz and Grey Golla

## Description
We are implementing a subset of the optimizations from [Dominance-Based Duplication Simulation (DBDS)](https://dl.acm.org/doi/pdf/10.1145/3168811) in LLVM. The paper proposes a method to allow code to be moved from one block into multiple predecessor blocks efficiently, enabling a variety of optimizations.

[Project proposal (PDF)](./Proposal.pdf)

## Tasks
- [x] Implement a Dominance Tree Walk of the procedure
- [ ] Implement the Duplication Simulation Traversal step within the walk
- [ ] \(75% goal) Implement Constant Folding for addition instructions
- [ ] Implenent Constant folding for other instructions
- [ ] Implement Strength Reduction
- [ ] \(100% goal) Implement Read Elimination
- [ ] Add heuristics to determine if proposed optimization is beneficial
- [ ] Implement Conditional Elimination
{:.tasklist}
