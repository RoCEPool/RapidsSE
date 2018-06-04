/*============================================================================*/
/* Handlers.cpp -- Queue/System/Thread Event Handlers                         */
/*============================================================================*/

#include "Threads.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/* queue level event handler                                                  */
/*----------------------------------------------------------------------------*/
bool Handlers::bQueueLevelHandler(Task* pTask)
{
  //
  // this handler is called from Thread::vLoop()
  // after a task returns to the thread
  //
  
  // get the thread vector
  ThreadVector* pThreadVector = gpAttachMap->m_pQueues->pThreadVector();
  
  // grab the thread vector lock
  pThreadVector->vGrabLock();
  
  i8 iThreads = pThreadVector->nItems();
  
  // find a thread that can run the task
  for (i8 iThread = 0; iThread < iThreads; iThread++) {
    
    // get the thread
    Thread* pThread = pThreadVector->pGet(iThread);
    
    // try and lock the thread
    bool bLocked = pThread->bGrabLock();
    
    // if unable to lock, move on to the next thread
    if ( ! bLocked )
      continue;
    
    // see if thread is waiting for a task
    if (pThread->eThreadState() == TC_WAITING) {
      
      // set task state to READYTORUN
      pThread->vThreadState(TC_READYTORUN);
      
      // set the task in the thread
      pThread->vTask(pTask);
      
      // notify the thread
      pThread->vSignalThread();
      
      // drop the thread lock
      pThread->vDropLock();
      
      // drop the thread vector lock
      pThreadVector->vDropLock();
      
      return true;
    }
    
    // drop the thread lock
    pThread->vDropLock();
  }
  
  // drop the thread vector lock
  pThreadVector->vDropLock();
  
  // didn't find a thread
  return false;
}

/*----------------------------------------------------------------------------*/
/* system level event handler                                                 */
/*----------------------------------------------------------------------------*/
bool Handlers::bSystemLevelHandler(Task* pTask)
{
  //
  // this handler is called from SEitf::bAppendRequest_R35()
  // task should be ready to run
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;

  eQR eQueue = pTask->eWhichQueue();
  
  if (QR_READY_QUEUE == eQueue) {
    pQueues->vScheduleReadyTask(pTask);
    return true;
  }
  
  if (eQueue >= 0 && eQueue < gxArguments.m_nSlots) {
    pQueues->vScheduleReadySlotTask(pTask);
    return true;
  }
  
  /*
  if (QR_SLEEPING_QUEUE == eQueue) {
    pQueues->vScheduleSleepingTask(pTask);
    return true;
  }
  
  if (QR_WAITING_QUEUE == eQueue) {
    pQueues->vScheduleWaitingTask(pTask);
    return true;
  }
  */
  
  vFatal("Handlers::bSystemLevelHandler: Invalid eQueue %lld"_Stz,
         (i8)eQueue);
  return false;
}

/*----------------------------------------------------------------------------*/
/* thread level event handler                                                 */
/*----------------------------------------------------------------------------*/
bool Handlers::bThreadLevelHandler(NP_Task* pNpTask)
{
  //
  // this handler is called from Thread::vLoop()
  // after a task returns to the thread
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Task* pTask = **pNpTask;
  *pNpTask = nullptr;
  
  // first remove this task from the running queue
  pQueues->vRemoveFromRunningQueue(pTask);
  
  // now add the task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TS_BLOCKED:
      // should never come to thread handler with a blocked task
      ASSERT(0);
      
    case TS_EXITED:
      // task has exited, just delete it
      if ( (*gpAttachMap->m_npBootTask != pTask) && (! pTask->bWordBody()) ) {
        delete pTask;
      }
      break;
      
    case TS_RUNNING:
      // should never come to thread handler with a running task
      ASSERT(0);
      
    case TS_SLEEPING:
      // put task into sleeping queue
      pQueues->vAddToSleepingQueue(pTask);
      break;
      
    case TS_WAITING_ACK:
      // put task into waiting queue
      pQueues->vAddToWaitingQueue(pTask);
      break;
      
    case TS_WAITING_SIGNAL:
      // whats the difference between WAITING_ACK and WAITING_SIGNAL ?
      ASSERT(0);
      
    case TS_YIELDED:
      // put task into a ready queue
      if (pTask->iPartition() == -1)
        pQueues->vAddToReadyQueue(pTask);
      else
        pQueues->vAddToReadySlotQueue(pTask);
      break;
      
    default:
      break;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
