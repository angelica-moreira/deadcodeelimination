#ifndef DEADCODEANALYSIS_DEADCODEANALYSIS_H_
#define DEADCODEANALYSIS_DEADCODEANALYSIS_H_

#include "llvm/Pass.h"
#include "llvm/PassAnalysisSupport.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "RangeAnalysis.h"

using namespace std;
using namespace llvm;

class DeadCodeAnalysis: public FunctionPass {
private:
  vector<BranchInst*> worklist;
  map<BranchInst*, bool> workListMap;

public:
  static char ID;
  DeadCodeAnalysis() : FunctionPass(ID) {}
  virtual ~DeadCodeAnalysis() {
  }

  void createBranchInst();
  void eliminateUnreacheableBlock();
  void createNonTerminatorUnreachable(Instruction* InsertAt);
  int solveIcmpInstruction(ICmpInst* iCmpInst, InterProceduralRA<Cousot> &ra);
  virtual void getAnalysisUsage(AnalysisUsage &AU) const;
  virtual bool runOnFunction(Function &function);
};

#endif /* DEADCODEANALYSIS_DEADCODEANALYSIS_H_ */
