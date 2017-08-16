/*============================================================================*/
/* ThreadQueue.cpp -- Thread List                                             */
/*============================================================================*/

#include "Threads.h"

extern Heap*        gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ThreadQueue* Threads::_pThreadQueue(void* aAddr)
{
  ThreadQueue* pThreadQueue = reinterpret_cast<ThreadQueue*>(aAddr);
  
  RASSERT( !pThreadQueue || CN_THREADQUEUE == pThreadQueue->eClass() );
  
  return pThreadQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadQueue::operator new(size_t iSize, ThreadQueue* pThreadQueue)
{
  RASSERT(iSize);
  return pThreadQueue;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadQueue::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: ThreadQueue class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadQueue::ThreadQueue(void)
: Object(CN_THREADQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
    
  _checkInitialised_(CN_THREADQUEUE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadQueue::~ThreadQueue(void)
{
}

/*----------------------------------------------------------------------------*/
/* allocate a thread                                                          */
/*----------------------------------------------------------------------------*/
Thread* ThreadQueue::pAllocThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  Thread* pThread = _pThread(Heap::pAllocate(CN_THREAD));
  
  vAppend(pThread);
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* free a thread                                                              */
/*----------------------------------------------------------------------------*/
void ThreadQueue::vFreeThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  vRemove(pThread);
  
  Heap::vFree(pThread);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ThreadQueue::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
