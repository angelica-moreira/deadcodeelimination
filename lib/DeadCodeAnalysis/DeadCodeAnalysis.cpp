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

void DeadCodeAnalysis::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
    AU.addRequired<InterProceduralRA<Cousot> >();
}

int DeadCodeAnalysis::solveIcmpInstruction(ICmpInst* iCmpInst, InterProceduralRA<Cousot> &ra){
  bool wrongRange;
  int removeInst = 0; // if =1 then true, if = 2, then false, if =0 then nothing
  Range r1 = ra.getRange(iCmpInst->getOperand(0));
  Range r2 = ra.getRange(iCmpInst->getOperand(1));

  switch(iCmpInst->getPredicate()){
    case CmpInst::ICMP_EQ:
      //remove the else part
      if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().eq(r2.getUpper()))){
        errs()<< " ICMP_EQ: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2;
      } else if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().ne(r2.getUpper()))){
        removeInst = 1; //remove the if part
      } else if(r1.getLower().sgt(r2.getUpper()) || r1.getUpper().slt(r2.getLower())){
        removeInst = 1; //remove the if part
      } 
    break;
    case CmpInst::ICMP_NE:
      //remove the else part
      if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().ne(r2.getUpper()))){
        errs()<< " ICMP_NE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2;
      } else if((r1.getLower().eq(r1.getUpper())) &&  (r2.getLower().eq(r2.getUpper())) && (r1.getLower().eq(r2.getUpper()))){
        removeInst = 1; //remove the if part
      } else if(r1.getLower().slt(r2.getUpper()) || r1.getUpper().sgt(r2.getLower())){
        removeInst = 2; //remove the if part
      } 
    break;
    case CmpInst::ICMP_SGE:
      if(r1.getUpper().slt(r2.getLower())){
        errs()<< " ICMP_SGE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      }else if(r1.getLower().sge(r2.getUpper())){
        errs()<< " ICMP_SGE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      } 
    break;
    case CmpInst::ICMP_SGT:
      if(r1.getUpper().sle(r2.getLower())){
        errs()<< " ICMP_SGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      }else if(r1.getLower().sgt(r2.getUpper())){
        errs()<< " ICMP_SGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      } 
    break;
    case CmpInst::ICMP_SLT:
      if(r1.getLower().sge(r2.getUpper())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      } else if(r1.getUpper().slt(r2.getLower())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      }
    break;
    case CmpInst::ICMP_SLE:
      if(r1.getLower().sgt(r2.getUpper())){
        errs()<< " ICMP_SLE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      } else if(r1.getUpper().sle(r2.getLower())){
        errs()<< " ICMP_SLE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      }
    break;
    case CmpInst::ICMP_UGE:
      if(r1.getLower().uge(r2.getUpper())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      } else if(r1.getUpper().ult(r2.getLower())){
        errs()<< " ICMP_SLT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      }
    break;
    case CmpInst::ICMP_UGT:
      if(r1.getUpper().ule(r2.getLower())){
        errs()<< " ICMP_UGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      }else if(r1.getLower().ugt(r2.getUpper())){
        errs()<< " ICMP_UGT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      } 
    break;
    case CmpInst::ICMP_ULE:
      if(r1.getLower().ugt(r2.getUpper())){
        errs()<< " ICMP_ULE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      } else if(r1.getUpper().ule(r2.getLower())){
        errs()<< " ICMP_ULE: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      }
    break;
    case CmpInst::ICMP_ULT:
      if(r1.getLower().uge(r2.getUpper())){
        errs()<< " ICMP_ULT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 1; //remove the if part
      } else if(r1.getUpper().ult(r2.getLower())){
        errs()<< " ICMP_ULT: "<< r1.getUpper() <<  " - "<< r2.getLower() << " \n";
        removeInst = 2; //remove the else part
      }
    break;
    default:
      removeInst = 0;
    break;
  }
  return removeInst;
}

void DeadCodeAnalysis::createNonTerminatorUnreachable(Instruction *InsertAt) {
  auto &Ctx = InsertAt->getContext();
  new StoreInst(ConstantInt::getTrue(Ctx),
                UndefValue::get(Type::getInt1PtrTy(Ctx)),
                InsertAt);
  
}

void DeadCodeAnalysis::eliminateUnreacheableBlock(){
    for(auto const& it : workListMap){
      BranchInst* branchInst= it.first;
      bool value = it.second;
      if(value){
        if(ICmpInst* iCmpInst = dyn_cast<ICmpInst>(branchInst->getCondition())){
          iCmpInst->replaceAllUsesWith(ConstantInt::get(iCmpInst->getType(), 0));
          RecursivelyDeleteTriviallyDeadInstructions(iCmpInst);
          ++InstructionsEliminated;
        }
      }else{
        BranchInst* newBranchInst = BranchInst::Create(branchInst->getSuccessor(0), branchInst);
        if(ICmpInst* iCmpInst = dyn_cast<ICmpInst>(branchInst->getCondition())){
          iCmpInst->replaceAllUsesWith(ConstantInt::get(iCmpInst->getType(), 1));
          RecursivelyDeleteTriviallyDeadInstructions(iCmpInst);
          ++InstructionsEliminated;
        }
      }
    }
}

bool DeadCodeAnalysis::runOnFunction(Function &function)
{ 
  InterProceduralRA<Cousot>::ID=0;
  InterProceduralRA<Cousot> &ra = getAnalysis<InterProceduralRA<Cousot>>();
  if (function.isDeclaration() || function.empty())
    return false;

    bool unreacheable = false;
    for (BasicBlock &basicBlock : function)
    {
     for (Instruction &instruction : basicBlock)
      { 
        if (BranchInst* branch_instruction = dyn_cast<BranchInst>(&instruction))
        {
          if (branch_instruction->isConditional()){
            if(ICmpInst* iCmpInst = dyn_cast<ICmpInst>(branch_instruction->getCondition())){
              if(solveIcmpInstruction(iCmpInst,ra) == 1){
                BasicBlock* nextBB = branch_instruction->getSuccessor(0);
                unreacheable = true;
                createNonTerminatorUnreachable(nextBB->getTerminator());
                workListMap[branch_instruction] = true;
                worklist.push_back(branch_instruction);
              } else if(solveIcmpInstruction(iCmpInst,ra) == 2){
                BasicBlock* nextBB = branch_instruction->getSuccessor(1);
                unreacheable = true;
                createNonTerminatorUnreachable(nextBB->getTerminator());
                workListMap[branch_instruction] = false;
                worklist.push_back(branch_instruction);
              }
            }
          }
        }
     }
    }
  
  eliminateUnreacheableBlock();

  return unreacheable;
}

char DeadCodeAnalysis::ID = 0;
static RegisterPass<DeadCodeAnalysis> X("dead-code-ra", "Dead Code Elimination with Range Analysis.");

