/*============================================================================*/
/* Division.cpp -- Pipe Stage-Partition                                       */
/*============================================================================*/

#include "Pipelines.h"


/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Division* Pipelines::_pDivision(void* aAddr)
{
  Division* pStage = reinterpret_cast<Division*>(aAddr);
  
  RASSERT_TYPE(pStage,CN_DIVISION);
  
  return pStage;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Division::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_DIVISION);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Division::operator delete(void* aAddr)
{
  Division* pStage = reinterpret_cast<Division*>(aAddr);
  
  HeapMem::vFree(pStage);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Division::Division(StageSpec* pStageSpec, i8 i_Div)
: Object(CN_DIVISION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIVISION,_FOOTER_);
  
  m_i_Div = i_Div;
  //m_oOpVec
  
  _checkInitialised_(CN_DIVISION);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DIVISION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Division::~Division(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIVISION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get opVec                                                                  */
/*----------------------------------------------------------------------------*/
OpVec* Division::pOpVec(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return &m_oOpVec;
}

/*----------------------------------------------------------------------------*/
/* get partition this stageNde needs to run on                                */
/*----------------------------------------------------------------------------*/
i8 Division::i_Ptn(void)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
  return m_i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* spawn operators for partitions of given node                               */
/*----------------------------------------------------------------------------*/
void Division::vSpawnOperators(i8 iSessionId, i8 iNode)
{
  _precondition_(_MAGIC_,CN_DIVISION,_FOOTER_);
  
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
__aFooterFunction(Division)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
