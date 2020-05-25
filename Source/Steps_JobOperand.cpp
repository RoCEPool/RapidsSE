/*============================================================================*/
/* Steps_JobXmit.cpp - job xmit                                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobOperand(void)
{
  vAddStep("Job_Operand.parentJob@"_Stz, &Step::vJob_OperandParentJobFetch);
  vAddStep("Job_Operand.pipeOut"_Stz,    &Step::vJob_OperandPipeOut);
}

/*----------------------------------------------------------------------------*/
/* Job_Operand.parentJob@ -- get parent job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_OperandParentJobFetch(void)
{
  ENTRY();
  
  Job_Operand* pJob_Operand = _pJob_Operand(m_pDataStack->pPopPtr(__func__));
  
  // find the parent participant job
  i8 iParentJob = pJob_Operand->iParentId();
  Job* pParentJob = Scheduler::pJob_FindParticipating(iParentJob);
  
  m_pDataStack->vPushPtr(pParentJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Operand.pipeOut -- pipe out the operand                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_OperandPipeOut(void)
{
  ENTRY();
  
  Job_Operand* pJob_Operand = _pJob_Operand(m_pDataStack->pPopPtr(__func__));
  
  // find the parent participant job
  i8 iParentJob = pJob_Operand->iParentId();
  Job* pParentJob = Scheduler::pJob_FindParticipating(iParentJob);

  // get the next stage the xmit is supposed to xmit to
  Stage* pStage = pParentJob->m_pPipeline->pGet(pJob_Operand->m_iNextStage);
  
  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(pJob_Operand->m_eTopologyX,
                               pJob_Operand->m_iNode,
                               pJob_Operand->m_iPartition,
                               pJob_Operand->m_iNextOp);
  
  //! ALKA come back to this. May need a m_bAppendLeft in pJob_Operand
  // append to left queue
  pOp->vAppendLeft(pJob_Operand->m_pOperand,m_pTask);
  
  // now delete the job_operand
  delete pJob_Operand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
