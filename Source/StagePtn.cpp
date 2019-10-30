/*============================================================================*/
/* StagePtn.cpp -- Pipe Stage-Partition                                       */
/*============================================================================*/

#include "Operators.h"


/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StagePtn* Operators::_pStagePtn(void* aAddr)
{
  StagePtn* pStage = reinterpret_cast<StagePtn*>(aAddr);
  
  RASSERT_TYPE(pStage,CN_STAGEPTN);
  
  return pStage;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* StagePtn::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STAGEPTN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void StagePtn::operator delete(void* aAddr)
{
  StagePtn* pStage = reinterpret_cast<StagePtn*>(aAddr);
  
  HeapMem::vFree(pStage);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StagePtn::StagePtn(StageSpec* pStageSpec, i8 i_Ptn)
: Object(CN_STAGEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  m_pStageSpec = pStageSpec;
  m_i_Ptn      = i_Ptn;
  //m_oOpVec
  
  _checkInitialised_(CN_STAGEPTN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StagePtn::~StagePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  // opVec has duplicate Op's from Stage
  // clear it here to avoid double delete
  m_oOpVec.vClear();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* link all operators of the stage partition                                  */
/*----------------------------------------------------------------------------*/
void StagePtn::vLinkOperators(StageVec* pStageVec)
{
  _precondition_(_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  Op* pOpNext = nullptr;
  Op* pOp = nullptr;
  
  // for each operator in the stagePtn
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems() - 1; i_Op++) {
    
    // get operator
    pOp = m_oOpVec.pGet(i_Op);
    
    // get the next operator
    pOpNext = m_oOpVec.pGet(i_Op+1);
    
    // link the two operators
    pOp->vNextLabel(pOpNext->m_sName);
    pOp->vOpOut(pOpNext);
  }
  
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

/*----------------------------------------------------------------------------*/
/* get opVec                                                                  */
/*----------------------------------------------------------------------------*/
OpVec* StagePtn::pOpVec(void)
{
  _precondition_(_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  return &m_oOpVec;
}

/*----------------------------------------------------------------------------*/
/* get partition this stageNde needs to run on                                */
/*----------------------------------------------------------------------------*/
i8 StagePtn::iPartition(void)
{
  _precondition_(_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  return m_i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* StagePtn::pStageSpec(void)
{
  _precondition_(_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* spawn operators for partitions of given node                               */
/*----------------------------------------------------------------------------*/
void StagePtn::vSpawnOperatorJobs(i8 iNode)
{
  _precondition_(_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  i8 iPtnNode = TablePtn::iPartitionToNode(m_i_Ptn);
  
  if (iPtnNode == iNode) {
    // for each operator in the stage ptn
    for (i8 i_Op = m_oOpVec.nItems() - 1; i_Op >= 0; i_Op--) {
      
      Op* pOp = m_oOpVec.pGet(i_Op);
      
      bool bSpawned = pOp->bSpawnJob();
      
      ASSERT(bSpawned);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StagePtn)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
