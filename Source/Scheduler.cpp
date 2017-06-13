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
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSemaphore.vMutexLock();
  
  //
  // **there will be no threads in bGetTask()**
  //
  
  // get number of queued threads
  i8 nQueuedThreads = m_xThreadQueue.iCount();
  
  // when there are queued threads, hand the task to first queued thread
  if (nQueuedThreads) {
    Thread* mpThread = m_xThreadQueue.npPop();
    ASSERT(mpThread);
    mpThread->m_npTask = mpTask;
    m_xSemaphore.vCondSignal();
    m_xSemaphore.vMutexUnlock();
    return;
  }

  // no available threads, so queue the task
  m_xTaskQueue.vAppend(mpTask);
 
  // allow another thread to continue
  m_xSemaphore.vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* queue given thread onto this schedulaers thread queue                      */
/*----------------------------------------------------------------------------*/
bool Scheduler::bGetTask(Threads::Thread* mpThread, bool bWait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_); _mp_(mpThread);
  
  ASSERT( ! mpThread->m_npTask );
  
  Task* mpTask = nullptr;
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSemaphore.vMutexLock();

  //
  // **there will be no threads in vEnqueueTask()**
  //
  
  i8 nTasksInQueue = m_xTaskQueue.iCount();
  
  // when there are queued tasks, hand the first task to this thread
  if (nTasksInQueue) {
    mpTask = m_xTaskQueue.npPop();
    ASSERT(mpTask);
    mpThread->m_npTask = mpTask;
    m_xSemaphore.vMutexUnlock();
    return true;
  }
  
  // when the thread is prepared to wait, put thread into queue
  if (bWait) {
    m_xThreadQueue.vAppend(mpThread);
    m_xSemaphore.vCondWait();
    m_xSemaphore.vMutexUnlock();
    ASSERT(mpThread->m_npTask);
    return true;
  }

  // allow another thread to continue
  m_xSemaphore.vMutexUnlock();
  
  // no available task and thread was not prepared to wait
  return false;
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
/* how many threads are currently queued?                                     */
/*----------------------------------------------------------------------------*/
i8 Scheduler::iQueuedThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  i8 iCount = m_xThreadQueue.iCount();
  
  return iCount;
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
