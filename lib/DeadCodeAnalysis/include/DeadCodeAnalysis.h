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
#include <list>
#include <map>

#include "RangeAnalysis.h"

using namespace std;
using namespace llvm;

class DeadCodeAnalysis: public FunctionPass {
private:
  map<Value*, string> valuesMap;
  list<string> successorsList;
  int instrCount= 0;
  static const string BASIC_BLOCK_PREFIX;
  static const string INSTRUCTION_PREFIX;
  static const string FUNCTION_PREFIX;

public:
  static char ID;
  DeadCodeAnalysis() : FunctionPass(ID) {}
  virtual ~DeadCodeAnalysis() {
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const;
  virtual bool runOnFunction(Function &function);
};

#endif /* DEADCODEANALYSIS_DEADCODEANALYSIS_H_ */
