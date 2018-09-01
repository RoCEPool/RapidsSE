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
void Handlers::vAckTaskHandler(Task* pTaskBeingAcked, Task* pTaskAcking)
{
  //
  // this handler is called from Task::vTaskAck()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Job* pJobBeingAcked = pTaskBeingAcked->pJob();
  Job* pJobAcking = pTaskAcking->pJob();
  
  i8 iRemaining;
  bool bDecider = pTaskBeingAcked->bDecAck(iRemaining);
  
  if ( (pJobBeingAcked && JT_NONE != pJobBeingAcked->eJobTrace()) ||
       (pJobAcking && JT_NONE != pJobAcking->eJobTrace())  ) {
    vTrace("Ack of %s task %p by %s task %p acks remaining %lld (%s)\n"_Stz,
           pJobBeingAcked ? (Stz*)pJobBeingAcked->m_pJobClass_s->m_aName : "<no job>"_Stz,
           pTaskBeingAcked,
           pJobAcking ? (Stz*)pJobAcking->m_pJobClass_s->m_aName : "<no job>"_Stz,
           pJobAcking,
           iRemaining,
           bDecider ? "decider" : "not decider");
  }
  
  if (bDecider)
    pQueues->vScheduleAckedTask(pTaskBeingAcked);
}

/*----------------------------------------------------------------------------*/
/* spawned handler                                                            */
/*----------------------------------------------------------------------------*/
void Handlers::vSpawnedTaskHandler(Task* pTask)
{
  //
  // this handler is called from SEitf::bSpawnJob()
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
    case TS_SEND:        pQueues->vScheduleSendTask(pTask);      break;
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
