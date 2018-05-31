/*============================================================================*/
/* ThreadVector.hpp -- Vector of Threads                                      */
/*============================================================================*/

#include "Threads.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ThreadVector* ThreadVector::_pThreadVector(void* aAddr)
{
  ThreadVector* pThreadVector = reinterpret_cast<ThreadVector*>(aAddr);
  
  RASSERT( ! pThreadVector || CN_THREADVECTOR == pThreadVector->eClass() );
  
  return pThreadVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadVector::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_THREADVECTOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadVector::operator delete(void* pAddr)
{
  ThreadVector* pThreadVector = reinterpret_cast<ThreadVector*>(pAddr);
  
  Heap::vFree(pThreadVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadVector::ThreadVector(void)
{
  _precondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
    
  _checkInitialised_(CN_THREADVECTOR);
  
  _postcondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadVector::~ThreadVector(void)
{
  _precondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ThreadVector::aFooter(void)
{
  _precondition_(_MAGIC_,CN_THREADVECTOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
