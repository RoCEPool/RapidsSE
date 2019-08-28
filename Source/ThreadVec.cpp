/*============================================================================*/
/* ThreadVec.hpp -- Vector of Threads                                         */
/*============================================================================*/

#include "Threads.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ThreadVec* ThreadVec::_pThreadVec(void* aAddr)
{
  ThreadVec* pThreadVec = reinterpret_cast<ThreadVec*>(aAddr);
  
  RASSERT_TYPE(pThreadVec,CN_THREADVEC);
  
  return pThreadVec;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadVec::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_THREADVEC);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadVec::operator delete(void* pAddr)
{
  ThreadVec* pThreadVec = reinterpret_cast<ThreadVec*>(pAddr);
  
  HeapMem::vFree(pThreadVec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadVec::ThreadVec(void)
: m_okLock(LK_THREAD_VECTOR,this)
{
  _precondition_(_MAGIC_,CN_THREADVEC,_FOOTER_);
  
  _checkInitialised_(CN_THREADVEC);
  
  _postcondition_(_MAGIC_,CN_THREADVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadVec::~ThreadVec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_THREADVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void ThreadVec::vDropLock(void)
{
  _precondition_(_MAGIC_,CN_THREADVEC,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void ThreadVec::vGrabLock(void)
{
  _precondition_(_MAGIC_,CN_THREADVEC,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ThreadVec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_THREADVEC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
