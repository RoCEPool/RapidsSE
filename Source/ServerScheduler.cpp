/*============================================================================*/
/* ServerScheduler.cpp - schedule tasks to server threads                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"

#include "Thread.hpp"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ServerScheduler* Schedulers::_pServerScheduler(void* aAddr)
{
  ServerScheduler* pServerScheduler = reinterpret_cast<ServerScheduler*>(aAddr);
  
  RASSERT( !pServerScheduler || CN_SERVERSCHEDULER == pServerScheduler->eClass() );
  
  return pServerScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ServerScheduler::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  ServerScheduler* pServerScheduler =
    reinterpret_cast<ServerScheduler*>(Heap::pAllocate(CN_SERVERSCHEDULER));
  
  return pServerScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ServerScheduler::operator new(size_t iSize, ServerScheduler* pServerScheduler)
{
  RASSERT(iSize);
  return pServerScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ServerScheduler::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: Scheduler class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ServerScheduler::ServerScheduler(void)
: Scheduler(CN_SERVERSCHEDULER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SERVERSCHEDULER,_FOOTER_);
  
  _checkInitialised_(CN_SERVERSCHEDULER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SERVERSCHEDULER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* hand given task to waiting thread or queue onto this schedulers task queue */
/*----------------------------------------------------------------------------*/
void ServerScheduler::vEnqueueTask_Mutex(Task* mpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SERVERSCHEDULER,_FOOTER_); _mp_(mpTask);
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSemaphore.vMutexLock();
  
  gpAttachMap->m_iSchEnqueueTask++;
  
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
    
    vTrace("Popped thread %p:%lld handed task %p\n"_Stz,
           mpThread,
           mpThread->iThreadNo(),
           mpTask
           );
    
    m_xSemaphore.vCondBroadcast();
    m_xSemaphore.vMutexUnlock();
    return;
  }

  // no available threads, so queue the task
  vTrace("Queueing task %p as no threads...%lld\n"_Stz,
         mpTask,
         m_xTaskQueue.iCount());
  m_xTaskQueue.vAppend(mpTask);
  
  gpAttachMap->m_iSchEnqueueTaskWait++;
  
  // allow another thread to continue
  m_xSemaphore.vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* queue given thread onto this schedulaers thread queue                      */
/*----------------------------------------------------------------------------*/
bool ServerScheduler::bGetTask_Mutex(Threads::Thread* mpThread, bool bWait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SERVERSCHEDULER,_FOOTER_); _mp_(mpThread);
  
  ASSERT( ! mpThread->m_npTask );
  
  Task* mpTask = nullptr;
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSemaphore.vMutexLock();
  gpAttachMap->m_iSchGetTask++;
  //
  // **there will be no threads in vEnqueueTask()**
  //
  
  i8 nTasksInQueue = m_xTaskQueue.iCount();
  
  // when there are queued tasks, hand the first task to this thread
  if (nTasksInQueue) {
    mpTask = m_xTaskQueue.npPop();
    ASSERT(mpTask);
    vTrace("Available thread %p:%lld handed task %p\n"_Stz,
           mpThread,
           mpThread->iThreadNo(),
           mpTask
           );
    mpThread->m_npTask = mpTask;
    m_xSemaphore.vMutexUnlock();
    return true;
  }
  
  // when the thread is prepared to wait, put thread into queue
  if (bWait) {
    gpAttachMap->m_iSchGetTaskWait++;
    vTrace("Queueing thread %p:%lld\n"_Stz,
           mpThread,
           mpThread->iThreadNo());
    m_xThreadQueue.vAppend(mpThread);
    do {
      m_xSemaphore.vCondWait_Mutex();
    } while (! *mpThread->m_npTask);
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
i8 ServerScheduler::iQueuedTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SERVERSCHEDULER,_FOOTER_);
  
  i8 nTasks = m_xTaskQueue.iCount();
  
  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* how many threads are currently queued?                                     */
/*----------------------------------------------------------------------------*/
i8 ServerScheduler::iQueuedThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SERVERSCHEDULER,_FOOTER_);
  
  i8 iCount = m_xThreadQueue.iCount();
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ServerScheduler::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SERVERSCHEDULER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
