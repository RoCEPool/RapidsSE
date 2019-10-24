/*============================================================================*/
/* StageNde.cpp -- Pipe Stage-Partition                                       */
/*============================================================================*/

#include "Operators.h"


/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StageNde* Operators::_pStageNde(void* aAddr)
{
  StageNde* pStage = reinterpret_cast<StageNde*>(aAddr);
  
  RASSERT_TYPE(pStage,CN_STAGENDE);
  
  return pStage;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* StageNde::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STAGENDE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void StageNde::operator delete(void* aAddr)
{
  StageNde* pStage = reinterpret_cast<StageNde*>(aAddr);
  
  HeapMem::vFree(pStage);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageNde::StageNde(StageSpec* pStageSpec, i8 i_Nde)
: Object(CN_STAGENDE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGENDE,_FOOTER_);
  
  m_pStageSpec = pStageSpec;
  m_i_Nde      = i_Nde;
  //m_oOpVec
  
  _checkInitialised_(CN_STAGENDE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGENDE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageNde::~StageNde(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGENDE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* link all operators of this stage node                                      */
/*----------------------------------------------------------------------------*/
void StageNde::vLinkOperators(StageVec* pStageVec)
{
  _precondition_(_MAGIC_,CN_STAGENDE,_FOOTER_);
  
  Op* pOpNext = nullptr;
  
  // for each operator in the partition
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems() - 1; i_Op++) {
    
    // get operator to be linked
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    if (pOpNext)
      pOpNext->vNextLabel(pOp->m_sName);
    
    // get the next operator
    pOpNext = m_oOpVec.pGet(i_Op+1);
    
    // link the two operators
    pOp->vNextLabel(pOpNext->m_sName);
    pOp->vOpOut(pOpNext);
    
    // get the last operator of this stage
    pOp = m_oOpVec.pGet(m_oOpVec.nItems()-1);
    
    // if last operator is xmit, link to the first operator of the next stage
    if (pOp->eClass() == CN_XMITOP) {
      
      // get the xmitOp's next stage
      RASSERT(pOp->m_iNextStage != C_UNKNOWN);
      Stage* pNextStage = pStageVec->pGet(pOp->m_iNextStage);
      RASSERT(pNextStage);
      
      pOp->vLinkToStage(pNextStage);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get node this stageNde needs to run on                                     */
/*----------------------------------------------------------------------------*/
i8 StageNde::iNode(void)
{
  _precondition_(_MAGIC_,CN_STAGENDE,_FOOTER_);
  
  return m_i_Nde;
}

/*----------------------------------------------------------------------------*/
/* get opVec                                                                  */
/*----------------------------------------------------------------------------*/
OpVec* StageNde::pOpVec(void)
{
  _precondition_(_MAGIC_,CN_STAGENDE,_FOOTER_);
  
  return &m_oOpVec;
}

/*----------------------------------------------------------------------------*/
/* spawn operators for given node                                             */
/*----------------------------------------------------------------------------*/
void StageNde::vSpawnOperatorJobs(Session* pSession, i8 iNode)
{
  _precondition_(_MAGIC_,CN_STAGENDE,_FOOTER_);
  
  if (m_i_Nde == iNode) {
    // for each operator in the stage nde
    for (i8 i_Op = m_oOpVec.nItems() - 1; i_Op >= 0; i_Op--) {
      Op* pOp = m_oOpVec.pGet(i_Op);
      
      //bool bSpawned = AM::bSpawnOp(pSession,pOp);
      
      bool bSpawned = pOp->bSpawnJob(pSession);
      
      ASSERT(bSpawned);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* StageNde::pStageSpec(void)
{
  _precondition_(_MAGIC_,CN_STAGENDE,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StageNde)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
