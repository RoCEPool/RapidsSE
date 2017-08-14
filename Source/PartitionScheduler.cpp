/*============================================================================*/
/* PartitionScheduler.cpp - schedule tasks to server threads                  */
/*============================================================================*/

#include "Schedulers.h"

extern BufferPool*  gpBufferPool;

#include "Thread.hpp"

extern i8 giSchEnqueueTask;
extern i8 giSchEnqueueTaskWait;
extern i8 giSchGetTask;
extern i8 giSchGetTaskWait;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PartitionScheduler* Schedulers::_pPartitionScheduler(void* aAddr)
{
  PartitionScheduler* pPartitionScheduler = reinterpret_cast<PartitionScheduler*>(aAddr);
  
  RASSERT( !pPartitionScheduler || CN_PARTITIONSCHEDULER == pPartitionScheduler->eClass() );
  
  return pPartitionScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PartitionScheduler::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  PartitionScheduler* pPartitionScheduler =
    reinterpret_cast<PartitionScheduler*>(Heap::pAllocate(CN_PARTITIONSCHEDULER));
  
  return pPartitionScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PartitionScheduler::operator new(size_t iSize, PartitionScheduler* pPartitionScheduler)
{
  RASSERT(iSize);
  return pPartitionScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PartitionScheduler::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: Scheduler class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PartitionScheduler::PartitionScheduler(void)
: Scheduler(CN_PARTITIONSCHEDULER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_);
  
  _checkInitialised_(CN_PARTITIONSCHEDULER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* hand given task to waiting thread or queue onto this schedulers task queue */
/*----------------------------------------------------------------------------*/
void PartitionScheduler::vEnqueueTask_Mutex(Task* mpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_); _mp_(mpTask);
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSemaphore.vMutexLock();
  
  giSchEnqueueTask++;
  
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
    m_xSemaphore.vCondBroadcast();
    m_xSemaphore.vMutexUnlock();
    return;
  }

  // no available threads, so queue the task
  m_xTaskQueue.vAppend(mpTask);
 
  giSchEnqueueTaskWait++;
  
  // allow another thread to continue
  m_xSemaphore.vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* queue given thread onto this schedulaers thread queue                      */
/*----------------------------------------------------------------------------*/
bool PartitionScheduler::bGetTask_Mutex(Threads::Thread* mpThread, bool bWait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_); _mp_(mpThread);
  
  ASSERT( ! mpThread->m_npTask );
  
  Task* mpTask = nullptr;
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSemaphore.vMutexLock();
  giSchGetTask++;
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
    giSchGetTaskWait++;
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
i8 PartitionScheduler::iQueuedTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_);
  
  i8 nTasks = m_xTaskQueue.iCount();
  
  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* how many threads are currently queued?                                     */
/*----------------------------------------------------------------------------*/
i8 PartitionScheduler::iQueuedThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_);
  
  i8 iCount = m_xThreadQueue.iCount();
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PartitionScheduler::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
