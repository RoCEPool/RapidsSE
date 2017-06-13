/*============================================================================*/
/* Scheduler.cpp - queue of tasks meets queue of threads                 */
/*============================================================================*/

#include "Schedulers.h"

extern BufferPool*  gpBufferPool;

#include "Thread.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Scheduler* Schedulers::_pScheduler(void* aAddr)
{
  Scheduler* pScheduler = reinterpret_cast<Scheduler*>(aAddr);
  
  RASSERT( !pScheduler || CN_SCHEDULER == pScheduler->eClass() );
  
  return pScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Scheduler::operator new(size_t iSize, Scheduler* pScheduler)
{
  RASSERT(iSize);
  return pScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: Scheduler class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Scheduler::Scheduler(void)
: Object(CN_SCHEDULER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  m_xLock.vInit("Scheduler::m_xLock"_Str);
  
  m_iCount = 0;
  m_npHead = nullptr;
  m_npTail = nullptr;
  
  _checkInitialised_(CN_SCHEDULER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* grab write lock on this task queue                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);

  m_xLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop write lock on this task queue                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* queue task onto this task queue                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::vQueueTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
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
i8 Scheduler::iQueuedTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  return m_iCount;
}

/*----------------------------------------------------------------------------*/
/* pop a task                                                                 */
/*----------------------------------------------------------------------------*/
Task* Scheduler::mpPopTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
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
void Scheduler::vQueueThread(Threads::Thread* npThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  m_xThreadQueue.vAppend(npThread);
}

/*----------------------------------------------------------------------------*/
/* how many threads are currently queued?                                     */
/*----------------------------------------------------------------------------*/
i8 Scheduler::iQueuedThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  i8 iCount = m_xThreadQueue.iCount();
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* pop a task                                                                 */
/*----------------------------------------------------------------------------*/
Thread* Scheduler::mpPopThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
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
void Scheduler::vVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
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
Footer* Scheduler::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
