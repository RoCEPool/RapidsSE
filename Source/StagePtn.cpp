/*============================================================================*/
/* StagePtn.cpp -- Pipe Stage-Partition                                       */
/*============================================================================*/

#include "Pipelines.h"


/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StagePtn* Pipelines::_pStagePtn(void* aAddr)
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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
i8 StagePtn::i_Ptn(void)
{
  _precondition_(_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  return m_i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* spawn operators for partitions of given node                               */
/*----------------------------------------------------------------------------*/
void StagePtn::vSpawnOperators(i8 iSessionId, i8 iNode)
{
  _precondition_(_MAGIC_,CN_STAGEPTN,_FOOTER_);
  
  i8 iPtnNode = TablePtn::iPartitionToNode(m_i_Ptn);
  
  if (iPtnNode == iNode) {
    // for each operator in the stage ptn
    for (i8 i_Op = m_oOpVec.nItems() - 1; i_Op >= 0; i_Op--) {
      
      Op* pOp = m_oOpVec.pGet(i_Op);
      
      bool bSpawned = pOp->bSpawnJob(iSessionId);
      
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
