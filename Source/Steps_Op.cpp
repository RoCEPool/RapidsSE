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
  vAddStep("Op.leftEosMarkers!"_Stz,  &Step::vOpLeftEosMarkersStore);
  vAddStep("Op.linkNextStage"_Stz,    &Step::vOpLinkNextStage);
  vAddStep("Op.nextStage!"_Stz,       &Step::vOpNextStageStore);
  vAddStep("Op.partition@"_Stz,       &Step::vOpPartitionFetch);
  vAddStep("Op.pipelineJob@"_Stz,     &Step::vOpPipleineJobFetch);
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
/* Op.linkNextStage -- link this operator to the next stage                   */
/*----------------------------------------------------------------------------*/
void Step::vOpLinkNextStage(void)
{
  ENTRY(); // ENTRY("vOpLinkNextStage"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  Pipeline* pPipeline = pOp->pPipeline();
  StageNde* pStageNde;
  Stage* pNextStage;
  Op* pOpNext;
  i8 i_Node;
  
  if (pOp->eTopologyX() != PX_ORIGIN_TO_ORIGIN) {
    RASSERT(pOp->pOpOut() != nullptr);
    goto L_EXIT;
  }

  RASSERT(pOp->eTopologyX() == PX_ORIGIN_TO_ORIGIN);
  
  // get the originating node
  i_Node = pPipeline->iOriginatingNode();
  
  // get the next stage
  pNextStage = pPipeline->pStageVec()->pGet(pOp->iNextStage());
  RASSERT(pNextStage->pStageNdeVec()->nItems() == 1) ;
  
  // get the stageNde of originating node
  pStageNde = pNextStage->pStageNde(i_Node);
  
  // get the first op of the stage nde
  pOpNext = pStageNde->pOpVec()->pGet(0);
  
  // set the next label
  pOp->vNextLabel(pOpNext->szName());
  
  // set the op out
  pOp->vOpOut(pOpNext);
  
L_EXIT:
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
void Step::vOpPipleineJobFetch(void)
{
  ENTRY(); // ENTRY("vOpPipleineJobFetch"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  m_pDataStack->vPushPtr(pOp->pPipeline()->pJob());
  
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
