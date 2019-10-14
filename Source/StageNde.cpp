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
void StageNde::vLinkOperators(void)
{
  _precondition_(_MAGIC_,CN_STAGENDE,_FOOTER_);
  
  Op* pOpNext = nullptr;
  
  // for each operator in the partition
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems() - 1; i_Op++) {
    
    // get operator to be linked
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    if (pOpNext)
      pOpNext->vNextLabel(pOp->szLabel());
    
    // get the next operator
    pOpNext = m_oOpVec.pGet(i_Op+1);
    
    // link the two operators
    pOp->vNextLabel(pOpNext->szLabel());
    pOp->vOpOut(pOpNext);
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
