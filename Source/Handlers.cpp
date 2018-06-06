/*============================================================================*/
/* Handlers.cpp -- Thread/Task Event Handlers                                 */
/*============================================================================*/

#include "Threads.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/* ack task handler                                                           */
/*----------------------------------------------------------------------------*/
bool Handlers::bAckTaskHandler(Task* pTask)
{
  //
  // this handler is called from Task::vTaskAck()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  bool bDecider = pTask->bDecAck();
  
  if (bDecider) {
    
    pQueues->vScheduleAckedTask(pTask);
    
    return true;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* spawned handler                                                            */
/*----------------------------------------------------------------------------*/
bool Handlers::bSpawnedTaskHandler(Task* pTask)
{
  //
  // this handler is called from SEitf::bAppendRequest_R35()
  // we want to run given task. task should be ready to run
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;

  pQueues->vScheduleSpawnedTask(pTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* thread loop handler                                                        */
/*----------------------------------------------------------------------------*/
bool Handlers::bThreadLoopHandler(NP_Task* pNpTask)
{
  //
  // this handler is called from Thread::vLoop()
  // after a task returns to the thread
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Task* pTask = **pNpTask;
  *pNpTask = nullptr;
  
  // first remove this task from the running queue
  pQueues->vRemoveFromExecutingQueue(pTask);
  
  // now add the task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TS_EXITED:      pQueues->vScheduleExitedTask(pTask);    break;
    case TS_SLEEPING:    pQueues->vScheduleSleepingTask(pTask);  break;
    case TS_WAITING_ACK: pQueues->vScheduleWaitingTask(pTask);   break;
    case TS_YIELDED:     pQueues->vScheduleYieldedTask(pTask);   break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // should never come to thread handler for these states

    case TS_BLOCKED:     ASSERT(0);
    case TS_EXECUTING:   ASSERT(0);
    case TS_WAITING_SIGNAL: ASSERT(0);
      
    default:
      break;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
