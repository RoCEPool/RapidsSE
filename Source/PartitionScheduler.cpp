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
  
  for (i8 i_PtnNo = 0; i_PtnNo < C_SLOTS_PER_NODE; i_PtnNo++) {
    m_xSlots[i_PtnNo].m_pSemaphore = nullptr;
    m_xSlots[i_PtnNo].m_pTaskQueue = nullptr;
    m_xSlots[i_PtnNo].m_pThreadQueue = nullptr;
  }
  
  _checkInitialised_(CN_PARTITIONSCHEDULER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* hand given task to waiting thread or queue onto this schedulers task queue */
/*----------------------------------------------------------------------------*/
void PartitionScheduler::vEnqueueTask_Mutex(i8 i_PtnNo, Task* mpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_); _mp_(mpTask);
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSlots[i_PtnNo].m_pSemaphore->vMutexLock();
  
  giSchEnqueueTask++;
  
  //
  // **there will be no threads in bGetTask()**
  //
  
  // get number of queued threads
  i8 nQueuedThreads = m_xSlots[i_PtnNo].m_pThreadQueue->iCount();
  
  // when there are queued threads, hand the task to first queued thread
  if (nQueuedThreads) {
    Thread* mpThread = m_xSlots[i_PtnNo].m_pThreadQueue->npPop();
    ASSERT(mpThread);
    mpThread->m_npTask = mpTask;
    m_xSlots[i_PtnNo].m_pSemaphore->vCondBroadcast();
    m_xSlots[i_PtnNo].m_pSemaphore->vMutexUnlock();
    return;
  }

  // no available threads, so queue the task
  m_xSlots[i_PtnNo].m_pTaskQueue->vAppend(mpTask);
 
  giSchEnqueueTaskWait++;
  
  // allow another thread to continue
  m_xSlots[i_PtnNo].m_pSemaphore->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* queue given thread onto this schedulers thread queue                       */
/*----------------------------------------------------------------------------*/
bool PartitionScheduler::bGetTask_Mutex(i8 i_PtnNo, Threads::Thread* mpThread, bool bWait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONSCHEDULER,_FOOTER_); _mp_(mpThread);
  
  ASSERT( ! mpThread->m_npTask );
  
  Task* mpTask = nullptr;
  
  // lock semaphore so only this thread is running
  // no other thread can be running the enqueue or get functions
  m_xSlots[i_PtnNo].m_pSemaphore->vMutexLock();
  giSchGetTask++;
  //
  // **there will be no threads in vEnqueueTask()**
  //
  
  i8 nTasksInQueue = m_xSlots[i_PtnNo].m_pTaskQueue->iCount();
  
  // when there are queued tasks, hand the first task to this thread
  if (nTasksInQueue) {
    mpTask = m_xSlots[i_PtnNo].m_pTaskQueue->npPop();
    ASSERT(mpTask);
    mpThread->m_npTask = mpTask;
    m_xSlots[i_PtnNo].m_pSemaphore->vMutexUnlock();
    return true;
  }
  
  // when the thread is prepared to wait, put thread into queue
  if (bWait) {
    giSchGetTaskWait++;
    m_xSlots[i_PtnNo].m_pThreadQueue->vAppend(mpThread);
    do {
      m_xSlots[i_PtnNo].m_pSemaphore->vCondWait_Mutex();
    } while (! *mpThread->m_npTask);
    m_xSlots[i_PtnNo].m_pSemaphore->vMutexUnlock();
    ASSERT(mpThread->m_npTask);
    return true;
  }

  // allow another thread to continue
  m_xSlots[i_PtnNo].m_pSemaphore->vMutexUnlock();
  
  // no available task and thread was not prepared to wait
  return false;
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
