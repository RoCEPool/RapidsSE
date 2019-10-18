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
void Step::vJob_OperandPipeOut(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_OperandPipeOut"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Operand.pipeOut"_Stz);
  Job_Operand* pJob_Operand = _pJob_Operand(pJobPtr);
  
  // find the parent participant job
  i8 iParentJob = pJob_Operand->m_iParentId;
  Job_Dynamic* pParentJob = _pJob_Dynamic(Scheduler::pFindParticipatingJob(iParentJob));

  // get the next stage the xmit is supposed to xmit to
  Stage* pStage = pParentJob->m_pPipeline->pStageVec()->pGet(pJob_Operand->m_iNextStage);
  
  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(pJob_Operand->m_eTopologyX, pJob_Operand->m_iNode);
  pOp->vAppendLeft(pJob_Operand->m_pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
