/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Op(void)
{
  vAddStep("Op::pipeout"_Stz,         &Step::vOpPipeOut);
  
  vAddStep("Op.ackPipelineJob"_Stz,   &Step::vOpAckPipelineJob);
  vAddStep("Op.nextOp!"_Stz,          &Step::vOpNextOpStore);
  vAddStep("Op.nextStage!"_Stz,       &Step::vOpNextStageStore);
  vAddStep("Op.partition@"_Stz,       &Step::vOpPartitionFetch);
  vAddStep("Op.pipelineJob@"_Stz,     &Step::vOpPiplineJobFetch);
  vAddStep("Op.stageName@"_Stz,       &Step::vOpStageNameFetch);
  vAddStep("Op.topology!"_Stz,        &Step::vOpTopologyStore);
  vAddStep("Op.xmitKind!"_Stz,        &Step::vOpXmitKindStore);
}

/*----------------------------------------------------------------------------*/
/* Op.ackPipelineJob -- ack the pipeline job                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpAckPipelineJob(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  pOp->vAckJob();
}

/*----------------------------------------------------------------------------*/
/* Op.nextOp! -- store next op in the operator                                */
/*----------------------------------------------------------------------------*/
void Step::vOpNextOpStore(void)
{
  ENTRY();
  
  m_pTask->m_pJob->m_pOp->vNextOp(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.nextStage! -- store next stage in the operator                          */
/*----------------------------------------------------------------------------*/
void Step::vOpNextStageStore(void)
{
  ENTRY();
  
  m_pTask->m_pJob->m_pOp->vNextStage(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.partition@ -- fetch partition from op                                   */
/*----------------------------------------------------------------------------*/
void Step::vOpPartitionFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pTask->m_pJob->m_pOp->nPartition());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.pipelineJob@ -- fetch pipeline job from op                              */
/*----------------------------------------------------------------------------*/
void Step::vOpPiplineJobFetch(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  m_pDataStack->vPushPtr(pOp->pPipeline()->pJob());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op::pipeout -- append operand onto next operator in pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeOut(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  pOp->vPipeOut_nextOp(pOperand,m_pTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.stageName@ -- fetch operator stage name                                 */
/*----------------------------------------------------------------------------*/
void Step::vOpStageNameFetch(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  s256 sStageName = pOp->szStageName();
  
  m_pDataStack->vPushStr(sStageName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.Topology! -- store topology in the operator                             */
/*----------------------------------------------------------------------------*/
void Step::vOpTopologyStore(void)
{
  ENTRY();
  
  m_pTask->m_pJob->m_pOp->vTopologyX((ePX)m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.xmitKind! -- store xmitKind in the operator                             */
/*----------------------------------------------------------------------------*/
void Step::vOpXmitKindStore(void)
{
  ENTRY();
  
  m_pTask->m_pJob->m_pOp->vXmitKind((eXK)m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
