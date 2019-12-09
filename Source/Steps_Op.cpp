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
  vAddStep("Op.leftEndMarkers!"_Stz,  &Step::vOpLeftEndMarkersStore);
  vAddStep("Op.linkNextStage"_Stz,    &Step::vOpLinkNextStage);
  vAddStep("Op.linkStartOp"_Stz,      &Step::vOpLinkStartOp);
  vAddStep("Op.nextStage!"_Stz,       &Step::vOpNextStageStore);
  vAddStep("Op.topology!"_Stz,        &Step::vOpTopologyStore);
}

/*----------------------------------------------------------------------------*/
/* Op.endMarkers! -- store end markers in op                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpLeftEndMarkersStore(void)
{
  ENTRY(); // ENTRY("vOpLeftEndMarkersStore"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  pOp->m_nLeftEndMarkers = m_pDataStack->iPopInt(__func__);
}

/*----------------------------------------------------------------------------*/
/* Op.linkNextStage -- link operator to next stage                            */
/*----------------------------------------------------------------------------*/
void Step::vOpLinkNextStage(void)
{
  ENTRY(); // ENTRY("vOpLinkNextStage"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  // link last operator to the first operator of the next stage if there is one
  ASSERT (pOp->m_iNextStage != C_UNKNOWN);
  Stage* pNextStage = pOp->m_pPipeline->pStageVec()->pGet(pOp->m_iNextStage);
  RASSERT(pNextStage);
    
  pOp->vLinkToStage(pNextStage);
}

/*----------------------------------------------------------------------------*/
/* Op.linkStartOp -- link operator to start operator                          */
/*----------------------------------------------------------------------------*/
void Step::vOpLinkStartOp(void)
{
  ENTRY(); // ENTRY("vOpLinkNextStage"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  ASSERT(AM::eJobType(pOp->m_pJobUniversal) == JT_STOPOP);
  
  // get the first stage
  Stage* pFirstStage = pOp->m_pPipeline->pStageVec()->pGet(0);
  
  // get the first op inm the first stage
  Op* pFirstOp = pFirstStage->pOpVec()->pGet(0);
  
  // this should be the stratOp
  ASSERT(AM::eJobType(pFirstOp->m_pJobUniversal) == JT_STARTOP);
 
  // set the opOut of stopOp to be the startOp
  pOp->vOpOut(pFirstOp);
  
}

/*----------------------------------------------------------------------------*/
/* Op.nextStage! -- store next stage in the operator                          */
/*----------------------------------------------------------------------------*/
void Step::vOpNextStageStore(void)
{
  ENTRY(); // ENTRY("vOpNextStageStore"_Stz);
  
  m_pTask->m_pJob->m_pOp->m_iNextStage = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* vOp.TopologyStore! -- store topology in the operator                       */
/*----------------------------------------------------------------------------*/
void Step::vOpTopologyStore(void)
{
  ENTRY(); // ENTRY("vOpTopologyStore"_Stz);
  
  m_pTask->m_pJob->m_pOp->m_eTopologyX = (ePX)m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
