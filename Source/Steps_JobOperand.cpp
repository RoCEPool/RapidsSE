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
void Step::vJob_OperandPipeOut(void)
{
  ENTRY(); // ENTRY("vJob_OperandPipeOut"_Stz);
  
  Object* pJobPtr;
  m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_Operand* pJob_Operand = _pJob_Operand(pJobPtr);
  
  // find the parent participant job
  i8 iParentJob = pJob_Operand->iParentId();
  Job* pParentJob = Scheduler::pFindParticipatingJob(iParentJob);

  // get the next stage the xmit is supposed to xmit to
  Stage* pStage = pParentJob->m_pPipeline->pStageVec()->pGet(pJob_Operand->m_iNextStage);
  
  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(pJob_Operand->m_eTopologyX, pJob_Operand->m_iNode);
  pOp->vAppendLeft(pJob_Operand->m_pOperand);
  
  // now delete the job_operand
  delete pJob_Operand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
