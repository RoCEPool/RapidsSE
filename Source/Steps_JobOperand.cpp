/*============================================================================*/
/* Steps_JobXmit.cpp - job xmit                                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobOperand(void)
{
  vAddStep("Job_Operand.pipeOut"_Stz,       &Step::vJob_OperandPipeOut);
}

/*----------------------------------------------------------------------------*/
/* Job_Operand.pipeOut -- pipe out the operand                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_OperandPipeOut(MP_Task mpTask)
{
  ENTRY("vJob_OperandPipeOut"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Operand.pipeOut"_Stz);
  Job_Operand* pJob_Operand = _pJob_Operand(pJobPtr);
  
  // find the parent participant job
  i8 iParentJob = pJob_Operand->m_iParentId;
  Job_Dynamic* pParentJob = _pJob_Dynamic(Scheduler::pFindParticipatingJob(iParentJob));
  
  // get the cross topology
  ePX eTopologyX = pJob_Operand->m_eTopologyX;
  
  // get the next stage the xmit is supposed to xmit to
  Stage* pStage = pParentJob->m_pPipeline->pStageVec()->pGet(pJob_Operand->m_iNextStage);
  
  if (pStage->pStageNdeVec()->nItems()) {
    StageNde* pStageNde = pStage->pStageNdeVec()->pGet(pJob_Operand->m_iNdeVec);
    Op* pOp = pStageNde->pOpVec()->pGet(0);
//    vTrace("***************%s::Job_Operand.pipeOut : %p %s\n"_Stz,
//           pOp->m_sName.szStz(),
//           pJob_Operand->m_pOperand,
//           aCN[pJob_Operand->m_pOperand->eClass()]);
    pOp->vAppendLeft(pJob_Operand->m_pOperand);
  }
  else {
    RASSERT(0);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
