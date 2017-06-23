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
  
  RASSERT( !pPredicateVec || CN_PREDICATEVEC == pPredicateVec->eClass() );
  
  return pPredicateVec;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PredicateVec::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_PREDICATEVEC);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PredicateVec::operator delete(void* pAddr)
{
  PredicateVec* pPredicateVec = reinterpret_cast<PredicateVec*>(pAddr);
  
  Heap::vFree(pPredicateVec);
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
PredicateVec::~PredicateVec(void)
{
  _precondition_(_MAGIC_,CN_PREDICATEVEC,_FOOTER_);
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
