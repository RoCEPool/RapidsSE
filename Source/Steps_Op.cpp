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
  vAddStep("Op.ackPipelineJob"_Stz,   &Step::vOpAckPipelineJob);
  vAddStep("Op.leftEndMarkers!"_Stz,  &Step::vOpLeftEndMarkersStore);
  vAddStep("Op.nextStage!"_Stz,       &Step::vOpNextStageStore);
  vAddStep("Op.partition@"_Stz,       &Step::vOpPartitionFetch);
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
/* Op.leftEndMarkers! -- store left end markers in op                         */
/*----------------------------------------------------------------------------*/
void Step::vOpLeftEndMarkersStore(void)
{
  ENTRY(); // ENTRY("vOpLeftEndMarkersStore"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  pOp->vLeftEndMarkers(m_pDataStack->iPopInt(__func__));
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
/* vOp.TopologyStore! -- store topology in the operator                       */
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
