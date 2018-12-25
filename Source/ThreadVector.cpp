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
  
  RASSERT_TYPE(pThreadVector,CN_THREADVECTOR);
  
  return pThreadVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadVector::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_THREADVECTOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadVector::operator delete(void* pAddr)
{
  ThreadVector* pThreadVector = reinterpret_cast<ThreadVector*>(pAddr);
  
  HeapMem::vFree(pThreadVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadVector::ThreadVector(void)
{
  _precondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
  
  m_oLock.vInitLock(LK_THREAD_VECTOR);
  
  _checkInitialised_(CN_THREADVECTOR);
  
  _postcondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadVector::~ThreadVector(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void ThreadVector::vDropLock(void)
{
  _precondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void ThreadVector::vGrabLock(void)
{
  _precondition_(_MAGIC_,CN_THREADVECTOR,_FOOTER_);
  
  m_oLock.vGrabWrite();
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
