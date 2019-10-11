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
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Operand.pipeOut"_Stz);
  Job_Operand* pJob_Operand = _pJob_Operand(pJobPtr);
  
  // find the parent participant job
  i8 iParentJob = pJob_Operand->m_iParentId;
  Job_Dynamic* pParentJob = _pJob_Dynamic(Scheduler::pFindParticipatingJob(iParentJob));
  
  Stage* pStage = pParentJob->m_pPipeline->m_oStageVec.pGet(pJob_Operand->m_iStage);
  StageNde* pStageNde = nullptr;
  
  if (pJob_Operand->m_bStageNdeVec) {
    pStageNde = pStage->m_oStageNdeVec.pGet(pJob_Operand->m_iStageNde);
  }
  else {
    RASSERT(0);
  }
  
  Op* pOp = pStageNde->m_oOpVec.pGet(0);
  //pJob_Operand->m_pOp->vPipeOut(pJob_Operand->m_pOperand);
  pOp->vAppendLeft(pJob_Operand->m_pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
