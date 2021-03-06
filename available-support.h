// 15-745 Assignment 2: available-support.h
// Group: Grey Golla and Kai Franz
////////////////////////////////////////////////////////////////////////////////

#ifndef __AVAILABLE_SUPPORT_H__
#define __AVAILABLE_SUPPORT_H__

#include <string>
#include <vector>

#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
std::string getShortValueName(Value *v);

class Expression {
public:
  Value *v1;
  Value *v2;
  Instruction::BinaryOps op;
  explicit Expression(Instruction *I);
  bool operator==(const Expression &e2) const;
  bool operator<(const Expression &e2) const;
  std::string toString() const;
  bool isKilledBy(Instruction *I) const;
};

void printSet(std::vector<Expression> *x);
} // namespace llvm

#endif
