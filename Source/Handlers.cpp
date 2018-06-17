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
void Handlers::vAckTaskHandler(Task* pTask)
{
  //
  // this handler is called from Task::vTaskAck()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  bool bDecider = pTask->bDecAck();
  
  if (bDecider)
    pQueues->vScheduleAckedTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* spawned handler                                                            */
/*----------------------------------------------------------------------------*/
void Handlers::vSpawnedTaskHandler(Task* pTask)
{
  //
  // this handler is called from SEitf::bAppendRequest_R35()
  // we want to run given task. task should be ready to run
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;

  pQueues->vScheduleSpawnedTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* thread loop handler                                                        */
/*----------------------------------------------------------------------------*/
void Handlers::vThreadLoopHandler(NP_Task* pNpTask)
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
    case TS_SEND:        pQueues->vScheduleSendTask(pTask);   break;
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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
