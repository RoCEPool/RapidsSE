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
  vAddStep("Op.pipelineJob@"_Stz,     &Step::vOpPipleineJobFetch);
  vAddStep("Op.stopStartOps"_Stz,     &Step::vOpStopStartOps);
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
/* Op.stopStartOps -- stop the startOps                                       */
/*----------------------------------------------------------------------------*/
void Step::vOpStopStartOps(void)
{
  ENTRY(); // ENTRY("vOpStopStartOps"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  Pipeline* pPipeline = pOp->pPipeline();
  
  // get the first stage
  Stage* pFirstStage = pPipeline->pStageVec()->pGet(0);
  
  StagePtn* pStagePtn;
  StageNde* pStageNde;
  Op* pOpNext;
  
  switch(pOp->eTopologyX()) {
    case PX_ORIGIN_TO_ALL_PARTITIONS: {

      for (i8 i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++) {
        
        // get the stage ptn
        pStagePtn = pFirstStage->pStagePtn(i_Ptn);
        
        // get the first op of the stage ptn
        pOpNext = pStagePtn->pOpVec()->pGet(0);
        RASSERT(AM::eJobType(pOpNext->pOpJob()) == JT_STARTOP);
        
        // link the stopOp to this startOp
        pOp->vOpOut(pOpNext);
        
        // send a stopMarker to the startOp
        pOp->vPipeOut(new StopMarker);
      }
      break;
    }
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      
      i8 i_Node = TablePtn::iPartitionToNode(pOp->nPartition());
      pStagePtn = pFirstStage->pStagePtn(pOp->nPartition());
      // get the first op of the stage ptn
      pOpNext = pStagePtn->pOpVec()->pGet(0);
        
      // link the stopOp to this startOp
      pOp->vOpOut(pOpNext);
      
      // send a stopMarker to the startOp
      pOp->vPipeOut(new StopMarker);

      break;
    }
      
    case PX_ORIGIN_TO_ORIGIN: {
  
      // get the stage nde
      pStageNde = pFirstStage->pStageNde(0);
    
      // get the first op of the stage node
      pOpNext = pStageNde->pOpVec()->pGet(0);
      RASSERT(AM::eJobType(pOpNext->pOpJob()) == JT_STARTOP);
    
      // link the stopOp to this startOp
      pOp->vOpOut(pOpNext);
    
      // send a stopMarker to the startOp
      pOp->vPipeOut(new StopMarker);
      break;
    }
      
    default:
      RASSERT(0);
  }
  
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
