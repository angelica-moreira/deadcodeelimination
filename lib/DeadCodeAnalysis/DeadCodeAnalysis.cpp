#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"

#include <iostream>
#include <fstream>

#include "include/DeadCodeAnalysis.h"

#define DEBUG_TYPE "DeadCodeAnalysis"

void DeadCodeAnalysis::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
    AU.addRequired<InterProceduralRA<Cousot> >();
}


bool DeadCodeAnalysis::runOnFunction(Function &function)
{ 
  InterProceduralRA<Cousot>::ID=0;
  InterProceduralRA<Cousot> &ra = getAnalysis<InterProceduralRA<Cousot>>();
  if (function.isDeclaration() || function.empty())
    return false;

    errs()<<"Function name: "<<function.getName()<<"\n";
    for (BasicBlock &basicBlock : function)
    {
      errs()<<"\n==================== Begin of Basic Block name: "<<basicBlock.getName()<<" =============\n";
      for (Instruction &instruction : basicBlock)
      {

        string strInst = instruction.getOpcodeName();

        Range r = ra.getRange(&instruction);
        if (!r.isUnknown()) {
          errs()<<"\n";
          errs()<<instruction<<"\n";
          errs()<<strInst<<"\n";
          r.print(errs());
          errs()<<"\n";
        } else
        {
          errs()<<"=================>>>>>> Unknown: "<<strInst<<"\n";
        }
      }
      errs()<<"\n==================== End of Basic Block name: "<<basicBlock.getName()<<" =============\n";
    }

  return false;
}

char DeadCodeAnalysis::ID = 0;
static RegisterPass<DeadCodeAnalysis> X("dead-code-ra", "Dead Code Elimination with Range Analysis.");
