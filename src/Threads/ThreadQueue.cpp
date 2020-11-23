/*============================================================================*/
/* ThreadQueue.hpp -- queue of (idle) threads                                 */
/*============================================================================*/

#include "Threads.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ThreadQueue* Threads::_pThreadQueue(void* aAddr)
{
  ThreadQueue* pThreadQueue = reinterpret_cast<ThreadQueue*>(aAddr);
  
  RASSERT_TYPE(pThreadQueue,CN_THREADQUEUE);
  
  return pThreadQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadQueue::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_THREADQUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadQueue::operator delete(void* aAddr)
{
  ThreadQueue* pThreadQueue = reinterpret_cast<ThreadQueue*>(aAddr);
  
  HeapMem::vFree(pThreadQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadQueue::ThreadQueue(void)
: ListNL_t<Thread>(CN_THREADQUEUE)
, m_okLock(LK_THREAD_QUEUE,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  _checkInitialised_(CN_THREADQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadQueue::~ThreadQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append thread to end of queue                                              */
/*----------------------------------------------------------------------------*/
void ThreadQueue::vAppendThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  ListNL_t<Thread>::vAppend(pThread);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void ThreadQueue::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void ThreadQueue::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool ThreadQueue::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool ThreadQueue::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at first thread at top of queue                                       */
/*----------------------------------------------------------------------------*/
Thread* ThreadQueue::pPeekThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Thread* pThread = ListNL_t<Thread>::pHead();

  return pThread;
}

/*----------------------------------------------------------------------------*/
/* pop first thread from top of queue                                         */
/*----------------------------------------------------------------------------*/
Thread* ThreadQueue::pPopThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Thread* pThread = ListNL_t<Thread>::pPop();

  return pThread;
}

/*----------------------------------------------------------------------------*/
/* get number of threads in queue                                             */
/*----------------------------------------------------------------------------*/
i8 ThreadQueue::nQueuedThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);

  i8 nThreads = ListNL_t<Thread>::nItems();

  return nThreads;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ThreadQueue::vContentDump(void)
{
  //vDump(CN_THREADQUEUE, this, Stz::szTab(1));
  
  ASSERT(m_okLock.bIsLocked());

  vTrace("ITEMS %lld\n"_Stz,ListNL_t<Thread>::nItems());
  
//  for (Thread* pThread = ListNL_t<Thread>::pHead();
//       pThread;
//       pThread = pThread->pNextNode()) {
//
//    vTrace("Thread %p %s %s\n"_Stz,
//           pThread,
//           aCN[pThread->pJob()->eClass()],
//           aTS[pThread->eState()]
//           );
//
//  }
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ThreadQueue);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
