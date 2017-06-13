/*============================================================================*/
/* QueryScheduler.cpp - queue of tasks meets queue of threads                 */
/*============================================================================*/

#include "Schedulers.h"

extern BufferPool*  gpBufferPool;

#include "Thread.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
QueryScheduler* Schedulers::_pTaskQueue(void* aAddr)
{
  QueryScheduler* pTaskQueue = reinterpret_cast<QueryScheduler*>(aAddr);
  
  RASSERT( !pTaskQueue || CN_TASKQUEUE == pTaskQueue->eClass() );
  
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* QueryScheduler::operator new(size_t iSize, QueryScheduler* pTaskQueue)
{
  RASSERT(iSize);
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void QueryScheduler::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: QueryScheduler class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryScheduler::QueryScheduler(void)
: Object(CN_TASKQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_xLock.vInit("QueryScheduler::m_xLock"_Str);
  
  m_iCount = 0;
  m_npHead = nullptr;
  m_npTail = nullptr;
  
  _checkInitialised_(CN_TASKQUEUE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* grab write lock on this task queue                                         */
/*----------------------------------------------------------------------------*/
void QueryScheduler::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);

  m_xLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop write lock on this task queue                                         */
/*----------------------------------------------------------------------------*/
void QueryScheduler::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* queue task onto this task queue                                            */
/*----------------------------------------------------------------------------*/
void QueryScheduler::vQueueTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  if ( ! m_npHead ) {
    // queue is empty
    ASSERT(0 == m_iCount);
    ASSERT( ! m_npTail );
    
    m_npHead  = pTask;
    m_npTail  = pTask;
    
    pTask->vNextInSet(nullptr);
    pTask->vPrevInSet(nullptr);
    
    m_iCount = 1;
  }
  else {
    // queue has task's
    ASSERT(m_iCount);
    ASSERT(m_npHead);
    ASSERT(m_npTail);
    
    pTask->vNextInSet(nullptr);
    pTask->vPrevInSet(m_npTail);
    
    m_npTail->vNextInSet(pTask);
    m_npTail = pTask;
    
    m_iCount++;
  }
}

/*----------------------------------------------------------------------------*/
/* how many tasks are currently queued?                                       */
/*----------------------------------------------------------------------------*/
i8 QueryScheduler::iQueuedTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  return m_iCount;
}

/*----------------------------------------------------------------------------*/
/* pop a task                                                                 */
/*----------------------------------------------------------------------------*/
Task* QueryScheduler::mpPopTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  if (m_npHead == nullptr)
    return nullptr;
    
  Task* pTask = *m_npHead;
  
  m_npHead = m_npHead->npNextInSet();
  
  if (m_npHead != nullptr)
    m_npHead->vPrevInSet(nullptr);
  else
    m_npTail = nullptr;
  
  m_iCount--;

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* queue thread onto this task queue                                          */
/*----------------------------------------------------------------------------*/
void QueryScheduler::vQueueThread(Threads::Thread* npThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  m_xThreadQueue.vAppend(npThread);
}

/*----------------------------------------------------------------------------*/
/* how many threads are currently queued?                                     */
/*----------------------------------------------------------------------------*/
i8 QueryScheduler::iQueuedThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  i8 iCount = m_xThreadQueue.iCount();
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* pop a task                                                                 */
/*----------------------------------------------------------------------------*/
Thread* QueryScheduler::mpPopThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  Threads::Thread* pThread = m_xThreadQueue.pPop();
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* verify sequence                                                            */
/*----------------------------------------------------------------------------*/
void QueryScheduler::vVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_xLock.vCheckWrite();
  
  if (m_npHead != nullptr) {
    ASSERT(!m_npHead->npPrevInSet());
  }
  else {
    ASSERT(!m_npTail);
  }

  if (m_npTail != nullptr) {
    ASSERT(!m_npTail->npNextInSet());
  }
  else {
    ASSERT(!m_npHead);
  }
  
  i8 iCount = 0;
  for (NP_Task npTask = m_npHead; npTask != nullptr ; npTask = npTask->npNextInSet()) {
    iCount++;
    if (npTask->npNextInSet() != nullptr) {
      ASSERT(*npTask == *(npTask->npNextInSet()->npPrevInSet()));
    }
    if (npTask->npPrevInSet() != nullptr) {
      ASSERT(*npTask == *(npTask->npPrevInSet()->npNextInSet()));
    }
  }
  
  ASSERT(iCount == m_iCount);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* QueryScheduler::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
