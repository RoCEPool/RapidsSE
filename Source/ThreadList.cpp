/*============================================================================*/
/* ThreadList.cpp - Thread Set                                                 */
/*============================================================================*/

#include "Threads.h"

extern Heap*        gpSmallHeap;
extern ThreadList*  gpThreadList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadList::operator new(size_t iSize, ThreadList* pThreadList)
{
  return pThreadList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadList::operator delete(void* pBufferAddress)
{
  vFatal("fatal: ThreadList class cannot be deleted");
  
  ASSERT(0);
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
  
  Memory::vCheckInitialised(this,sizeof(ThreadList));
  
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
  
  Thread* pThread = reinterpret_cast<Thread*>(gpSmallHeap->pAllocObject(CN_THREADLIST));
  
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
  
  gpSmallHeap->vFreeObject(pThread);
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