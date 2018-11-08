/*============================================================================*/
/* PredicateVec.hpp -- vector of predicates                                   */
/*============================================================================*/

#include "Plans.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PredicateVec* PredicateVec::_pPredicateVec(void* aAddr)
{
  PredicateVec* pPredicateVec = reinterpret_cast<PredicateVec*>(aAddr);
  
  RASSERT_TYPE(pPredicateVec,CN_PREDICATEVEC);
  
  return pPredicateVec;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PredicateVec::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_PREDICATEVEC);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PredicateVec::operator delete(void* pAddr)
{
  PredicateVec* pPredicateVec = reinterpret_cast<PredicateVec*>(pAddr);
  
  HeapMem::vFree(pPredicateVec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PredicateVec::PredicateVec(void)
{
  _precondition_(_MAGIC_,CN_PREDICATEVEC,_FOOTER_);
  
  m_nMatchedPredicates = 0;
  
  for (i8 i_Pred = 0; i_Pred < C_MAX_INDEX_COLUMNS; i_Pred++)
    m_bMatchedPredicate[i_Pred] = false;
  
  _checkInitialised_(CN_PREDICATEVEC);
  
  _postcondition_(_MAGIC_,CN_PREDICATEVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PredicateVec::~PredicateVec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_PREDICATEVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void PredicateVec::vSetMatched(i8 i_Pred)
{
  _precondition_(_MAGIC_,CN_PREDICATEVEC,_FOOTER_);
  
  if ( ! m_bMatchedPredicate[i_Pred]) {
    m_bMatchedPredicate[i_Pred] = true;
    m_nMatchedPredicates++;
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
bool PredicateVec::bFullyMatched(void)
{
  _precondition_(_MAGIC_,CN_PREDICATEVEC,_FOOTER_);
  
  if (m_nMatchedPredicates == nItems())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PredicateVec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_PREDICATEVEC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
