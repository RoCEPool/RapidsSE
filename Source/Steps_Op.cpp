/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"
#include "ePX.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Op(void)
{
  vAddStep("Op::pipeout"_Stz,         &Step::vOpPipeOut);
  
  vAddStep("Op.ackPipelineJob"_Stz,   &Step::vOpAckPipelineJob);
  vAddStep("Op.leftEosMarkers!"_Stz,  &Step::vOpLeftEosMarkersStore);
  vAddStep("Op.nextOp!"_Stz,          &Step::vOpNextOpStore);
  vAddStep("Op.nextStage!"_Stz,       &Step::vOpNextStageStore);
  vAddStep("Op.partition@"_Stz,       &Step::vOpPartitionFetch);
  vAddStep("Op.pipelineJob@"_Stz,     &Step::vOpPiplineJobFetch);
  vAddStep("Op.topology!"_Stz,        &Step::vOpTopologyStore);
}

/*----------------------------------------------------------------------------*/
/* Op.ackPipelineJob -- ack the pipeline job                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpAckPipelineJob(void)
{
  ENTRY(); // ENTRY("vOpAckPipelineJob"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  pOp->vAckJob();
}

/*----------------------------------------------------------------------------*/
/* Op.leftEosMarkers! -- store left end markers in op                         */
/*----------------------------------------------------------------------------*/
void Step::vOpLeftEosMarkersStore(void)
{
  ENTRY(); // ENTRY("vOpLeftEosMarkersStore"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  pOp->vLeftEosMarkers(m_pDataStack->iPopInt(__func__));
}

/*----------------------------------------------------------------------------*/
/* Op.nextOp! -- store next op in the operator                                */
/*----------------------------------------------------------------------------*/
void Step::vOpNextOpStore(void)
{
  ENTRY(); // ENTRY("vOpNextOpStore"_Stz);
  
  m_pTask->m_pJob->m_pOp->vNextOp(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.nextStage! -- store next stage in the operator                          */
/*----------------------------------------------------------------------------*/
void Step::vOpNextStageStore(void)
{
  ENTRY(); // ENTRY("vOpNextStageStore"_Stz);
  
  m_pTask->m_pJob->m_pOp->vNextStage(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.partition@ -- fetch partition from op                                   */
/*----------------------------------------------------------------------------*/
void Step::vOpPartitionFetch(void)
{
  ENTRY(); // ENTRY("vOpPartitionFetch"_Stz);
  
  m_pDataStack->vPushInt(m_pTask->m_pJob->m_pOp->nPartition());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.pipelineJob@ -- fetch pipeline job from op                              */
/*----------------------------------------------------------------------------*/
void Step::vOpPiplineJobFetch(void)
{
  ENTRY(); // ENTRY("vOpPipleineJobFetch"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  m_pDataStack->vPushPtr(pOp->pPipeline()->pJob());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op::pipeout -- append operand onto next operator in pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeOut(void)
{
  ENTRY(); // ENTRY("vOpPipleOut"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  pOp->vPipeOut(pOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.Topology! -- store topology in the operator                             */
/*----------------------------------------------------------------------------*/
void Step::vOpTopologyStore(void)
{
  ENTRY(); // ENTRY("vOpTopologyStore"_Stz);
  
  m_pTask->m_pJob->m_pOp->vTopologyX((ePX)m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
