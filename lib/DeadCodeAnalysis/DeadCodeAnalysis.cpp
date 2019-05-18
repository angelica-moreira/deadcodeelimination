#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/InstCombine/InstCombineWorklist.h"

#include "include/DeadCodeAnalysis.h"

#define DEBUG_TYPE "DeadCodeAnalysis"

STATISTIC(InstructionsEliminated, "Number of instructions eliminated");

void DeadCodeAnalysis::getAnalysisUsage(AnalysisUsage &AU) const{
  AU.setPreservesAll();
  AU.addRequired<InterProceduralRA<Cousot> >();
}

int DeadCodeAnalysis::solveIcmpInstruction(ICmpInst* iCmpInst, InterProceduralRA<Cousot> &ra){
  /// removeIst = 0 - no optimisation can be performed, 
  /// removeIst = 1 - the true path (if.then / if-path) will be removed, 
  /// removeIst = 2 - the false path (if.end / else-path) will be removed.
  int removeInst = 0; 
  Range r1 = ra.getRange(iCmpInst->getOperand(0));
  Range r2 = ra.getRange(iCmpInst->getOperand(1));

  switch(iCmpInst->getPredicate()){
    case CmpInst::ICMP_EQ:
      //it removes the else-path
      if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().eq(r2.getUpper()))){
        errs()<< " ICMP_EQ: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2;
      } else if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().ne(r2.getUpper()))){
        removeInst = 1; //it removes the if-path
      } else if(r1.getLower().sgt(r2.getUpper()) || r1.getUpper().slt(r2.getLower())){
        removeInst = 1; //it removes the if-path
      } 
    break;
    case CmpInst::ICMP_NE:
      //it removes the else-path
      if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().ne(r2.getUpper()))){
        errs()<< " ICMP_NE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2;
      } else if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().eq(r2.getUpper()))){
        removeInst = 1; //it removes the if-path
      } else if(r1.getLower().slt(r2.getUpper()) || r1.getUpper().sgt(r2.getLower())){
        removeInst = 2; //it removes the if-path
      } 
    break;
    case CmpInst::ICMP_SGE:
      if(r1.getUpper().slt(r2.getLower())){
        errs()<< " ICMP_SGE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      }else if(r1.getLower().sge(r2.getUpper())){
        errs()<< " ICMP_SGE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      } 
    break;
    case CmpInst::ICMP_SGT:
      if(r1.getUpper().sle(r2.getLower())){
        errs()<< " ICMP_SGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      }else if(r1.getLower().sgt(r2.getUpper())){
        errs()<< " ICMP_SGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      } 
    break;
    case CmpInst::ICMP_SLT:
      if(r1.getLower().sge(r2.getUpper())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      } else if(r1.getUpper().slt(r2.getLower())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      }
    break;
    case CmpInst::ICMP_SLE:
      if(r1.getLower().sgt(r2.getUpper())){
        errs()<< " ICMP_SLE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      } else if(r1.getUpper().sle(r2.getLower())){
        errs()<< " ICMP_SLE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      }
    break;
    case CmpInst::ICMP_UGE:
      if(r1.getLower().uge(r2.getUpper())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      } else if(r1.getUpper().ult(r2.getLower())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      }
    break;
    case CmpInst::ICMP_UGT:
      if(r1.getUpper().ule(r2.getLower())){
        errs()<< " ICMP_UGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      }else if(r1.getLower().ugt(r2.getUpper())){
        errs()<< " ICMP_UGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      } 
    break;
    case CmpInst::ICMP_ULE:
      if(r1.getLower().ugt(r2.getUpper())){
        errs()<< " ICMP_ULE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      } else if(r1.getUpper().ule(r2.getLower())){
        errs()<< " ICMP_ULE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      }
    break;
    case CmpInst::ICMP_ULT:
      if(r1.getLower().uge(r2.getUpper())){
        errs()<< " ICMP_ULT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //it removes the if-path
      } else if(r1.getUpper().ult(r2.getLower())){
        errs()<< " ICMP_ULT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //it removes the else-path
      }
    break;
    default:
      removeInst = 0;
    break;
  }
  return removeInst;
}

void DeadCodeAnalysis::markBlockAsUnrecheable(Instruction *InsertAt){
  auto &Ctx = InsertAt->getContext();
  new StoreInst(ConstantInt::getTrue(Ctx),
                UndefValue::get(Type::getInt1PtrTy(Ctx)),
                InsertAt);
  
}

void DeadCodeAnalysis::eliminateUnnecessaryInstruction(){
  for(auto const& it : workListMap){
    BranchInst* branchInst= it.first;
    Tuple tuple = it.second;
    ICmpInst* iCmpInst = tuple.first;
    bool value = tuple.second;
    if(value){
      iCmpInst->replaceAllUsesWith(ConstantInt::get(iCmpInst->getType(), 0));
      RecursivelyDeleteTriviallyDeadInstructions(iCmpInst);
      ++InstructionsEliminated;
    }else{
      iCmpInst->replaceAllUsesWith(ConstantInt::get(iCmpInst->getType(), 1));
      RecursivelyDeleteTriviallyDeadInstructions(iCmpInst);
      ++InstructionsEliminated;
    }
  }
}

bool DeadCodeAnalysis::runOnFunction(Function &function){ 
  InterProceduralRA<Cousot>::ID=0;
  InterProceduralRA<Cousot> &ra = getAnalysis<InterProceduralRA<Cousot>>();
  if (function.isDeclaration() || function.empty())
    return false;

  bool changed = false;
  for (BasicBlock &basicBlock : function){
    for (Instruction &instruction : basicBlock){ 
      if (BranchInst* branchInst = dyn_cast<BranchInst>(&instruction)){
        if (branchInst->isConditional()){
          if(ICmpInst* iCmpInst = dyn_cast<ICmpInst>(branchInst->getCondition())){
            if(solveIcmpInstruction(iCmpInst,ra) == 1){
              BasicBlock* nextBB = branchInst->getSuccessor(0);
              markBlockAsUnrecheable(nextBB->getTerminator());
              Tuple tuple = std::make_pair(iCmpInst, true);
              workListMap[branchInst] = tuple;
              changed = true;
            } else if(solveIcmpInstruction(iCmpInst,ra) == 2){
              BasicBlock* nextBB = branchInst->getSuccessor(1);
              markBlockAsUnrecheable(nextBB->getTerminator());
              Tuple tuple = std::make_pair(iCmpInst, false);
              workListMap[branchInst] = tuple;
              changed = true;
            }
          }
        }
      }
    }
  }
  
  eliminateUnnecessaryInstruction();

  return changed;
}

char DeadCodeAnalysis::ID = 0;
static RegisterPass<DeadCodeAnalysis> X("dead-code-ra", "Dead Code Elimination with Range Analysis.");

