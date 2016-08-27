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
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadList::ThreadList(void)
: Object(CN_THREADLIST)
{
  _poolAddress_(this);
  
  ASSERT( ! gpThreadList );
  
  gpThreadList = this;
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
  _poolAddress_(this);

  Thread* pThread = reinterpret_cast<Thread*>(gpSmallHeap->pAllocObject());
  
  vAppend(pThread);
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* free a thread                                                              */
/*----------------------------------------------------------------------------*/
void ThreadList::vFreeThread(Thread* pThread)
{
  _poolAddress_(this);
  
  vRemove(pThread);
  
  gpSmallHeap->vFreeObject(pThread);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/