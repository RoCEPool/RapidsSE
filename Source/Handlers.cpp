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
  
  *pNpTask = nullptr;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
