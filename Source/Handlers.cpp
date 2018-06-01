/*============================================================================*/
/* Handlers.cpp -- Queue/System/Thread Event Handlers                         */
/*============================================================================*/

#include "Threads.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* queue level handler                                                        */
/*----------------------------------------------------------------------------*/
bool Handlers::bQueueLevelHandler(Task* pTask)
{
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
/* system level handler                                                       */
/*----------------------------------------------------------------------------*/
bool Handlers::bSystemLevelHandler(Task* pTask)
{
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* thread level handler                                                       */
/*----------------------------------------------------------------------------*/
bool Handlers::bThreadLevelHandler(NP_Task* pNpTask)
{
  
  Task* pTask = **pNpTask;
  *pNpTask = nullptr;
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  // first remove this task from the running queue
  pQueues->vRemoveFromRunningQueue(pTask);
  
  // now add the task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TS_BLOCKED:
      // should never come to thread handler with a blocked task
      ASSERT(0);
      
    case TS_EXITED:
      // task has exited, just delete it
      if ( (*gpAttachMap->m_npBootTask != pTask)  && ( ! pTask->bWordBody()) ) {
        delete pTask;
      }
      break;
      
    case TS_RUNNING:
      // should never come to thread handler with a running task
      ASSERT(0);
      
    case TS_SLEEPING:
      // add task to sleeoping queue
      pQueues->vAddToSleepingQueue(pTask);
      break;
      
    case TS_WAITING_ACK:
      // add task to waiting queue
      pQueues->vAddToSuspendedQueue(pTask);
      break;
      
    case TS_WAITING_SIGNAL:
      // whats the difference between WAITING_ACK and WAITING_SIGNAL ?
      ASSERT(0);
      
    case TS_YIELDED:
      // add task back to the general/slot queue
      if (pTask->iPartition() == -1)
        pQueues->vAddToGeneralQueue(pTask);
      else
        pQueues->vAddToSlotQueue(pTask);
      break;
      
    default:
      break;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
