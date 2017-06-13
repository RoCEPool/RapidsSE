/*============================================================================*/
/* Scheduler.cpp - schedule tasks to threads                                  */
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
void* Scheduler::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Scheduler* pScheduler =
    reinterpret_cast<Scheduler*>(Heap::pAllocate(CN_SCHEDULER));
  
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
  
  
  _checkInitialised_(CN_SCHEDULER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* hand given task to waiting thread or queue onto this schedulers task queue */
/*----------------------------------------------------------------------------*/
void Scheduler::vEnqueueTask(Task* mpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_); _mp_(mpTask);
  
  vGrabLock();
  
  i8 iThreads = m_xThreadQueue.iCount();
  if (iThreads) {
    Thread* mpThread = m_xThreadQueue.npPop();
    ASSERT(mpThread);
    mpThread->m_npTask = mpTask;
    mpThread->m_pSemaphore->vSignalThread();
  }
  else {
    m_xTaskQueue.vAppend(mpTask);
  }
  
  vDropLock();
}

/*----------------------------------------------------------------------------*/
/* how many tasks are currently queued within this scheduler?                 */
/*----------------------------------------------------------------------------*/
i8 Scheduler::iQueuedTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);

  i8 nTasks = m_xTaskQueue.iCount();
  
  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* queue given thread onto this schedulaers thread queue                      */
/*----------------------------------------------------------------------------*/
void Scheduler::vEnqueueThread(Threads::Thread* mpThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_); _mp_(mpThread);
  
  vGrabLock();
  
  i8 iTasks = m_xTaskQueue.iCount();
  if (iTasks) {
    Task* mpTask = m_xTaskQueue.npPop();
    ASSERT(mpTask);
    mpThread->m_npTask = mpTask;
    mpThread->m_pSemaphore->vSignalThread();
  }
  else {
    m_xThreadQueue.vAppend(mpThread);
  }
  
  vDropLock();
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* grab write lock on this scheduler                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  m_xLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop write lock on this scheduler                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  m_xLock.vDropWrite();
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
