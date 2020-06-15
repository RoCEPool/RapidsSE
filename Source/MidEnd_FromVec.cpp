/*============================================================================*/
/* MidEnd_FromVec.hpp -- vector of from stream def's                          */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
MidEnd_FromVec* LogicalPlans::_pMidEnd_FromVec(void* aAddr)
{
  MidEnd_FromVec* pMidEnd_FromVec = reinterpret_cast<MidEnd_FromVec*>(aAddr);
  
  RASSERT_TYPE(pMidEnd_FromVec,CN_MIDEND_FROMVEC);
  
  return pMidEnd_FromVec;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* MidEnd_FromVec::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_MIDEND_FROMVEC);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void MidEnd_FromVec::operator delete(void* pAddr)
{
  MidEnd_FromVec* pMidEnd_FromVec = reinterpret_cast<MidEnd_FromVec*>(pAddr);
  
  HeapMem::vFree(pMidEnd_FromVec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MidEnd_FromVec::MidEnd_FromVec(void)
{
  _precondition_(_MAGIC_,CN_MIDEND_FROMVEC,_FOOTER_);
    
  _checkInitialised_(CN_MIDEND_FROMVEC);
  
  _postcondition_(_MAGIC_,CN_MIDEND_FROMVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MidEnd_FromVec::~MidEnd_FromVec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_MIDEND_FROMVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* MidEnd_FromVec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_MIDEND_FROMVEC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
