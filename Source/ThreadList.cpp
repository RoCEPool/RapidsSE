/*============================================================================*/
/* ThreadList.cpp - Thread Set                                                 */
/*============================================================================*/

#include "Threads.h"

extern Heap*        gpSmallHeap;
extern ThreadList*  gpThreadList;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ThreadList* Threads::_pThreadList(void* pAddr)
{
  ThreadList* pThreadList = reinterpret_cast<ThreadList*>(pAddr);
  
  return pThreadList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadList::operator new(size_t iSize, ThreadList* pThreadList)
{
  RASSERT(iSize);
  return pThreadList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadList::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: ThreadList class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadList::ThreadList(void)
: Object(CN_THREADLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADLIST,_FOOTER_);
  
  ASSERT( ! gpThreadList );
  
  gpThreadList = this;
  
  _checkInitialised_(CN_THREADLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREADLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadList::~ThreadList(void)
{
}

/*----------------------------------------------------------------------------*/
/* allocate a thread                                                          */
/*----------------------------------------------------------------------------*/
Thread* ThreadList::pAllocThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADLIST,_FOOTER_);
  
  Thread* pThread = _pThread(Heap::pAllocate(CN_THREAD));
  
  vAppend(pThread);
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* free a thread                                                              */
/*----------------------------------------------------------------------------*/
void ThreadList::vFreeThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADLIST,_FOOTER_);
  
  vRemove(pThread);
  
  Heap::vFree(pThread);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ThreadList::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/