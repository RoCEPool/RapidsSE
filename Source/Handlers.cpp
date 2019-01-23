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
bool Handlers::bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking)
{
  //
  // this handler is called from Task::vTaskAck()
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  ActivityMap::vMark(0,"pTaskBeingAcked:%p pTaskAcking:%p "_Stz,
                     pTaskBeingAcked,
                     pTaskAcking
                     );
  
  ActivityMap::vMark(0,"pJobAcking:%s "_Stz,
                     aCN[pTaskAcking->pJob()->eClass()]
                     );
  
//  ActivityMap::vMark(0,"pJobBeingAcked:%s "_Stz,
//                     aCN[pTaskBeingAcked->pJob()->eClass()]
//                     );
  
  Job* pJobBeingAcked = pTaskBeingAcked->pJob();
  Job* pJobAcking = pTaskAcking->pJob();
  
  bool bAmDecider = pTaskBeingAcked->bAckAndDecide();
  
  if (bAmDecider) {
    if (pJobBeingAcked->m_bMark || pJobAcking->m_bMark)
      ActivityMap::vMark(pJobAcking->m_iJobId,"Decider %s acks %s"_Stz,
                         aCN[pJobAcking->eClass()],
                         aCN[pJobBeingAcked->eClass()]
                         );
  }

  if ( (pJobBeingAcked && JT_NONE != pJobBeingAcked->eJobTrace()) ||
       (pJobAcking && JT_NONE != pJobAcking->eJobTrace())  ) {
    vTrace("Ack of %s task %p by %s task %p ; %s\n"_Stz,
           pJobBeingAcked ? (Stz*)pJobBeingAcked->m_pJobClass->m_aName : "<no job>"_Stz,
           pTaskBeingAcked,
           pJobAcking ? (Stz*)pJobAcking->m_pJobClass->m_aName : "<no job>"_Stz,
           pJobAcking,
           bAmDecider ? "decider" : "not decider");
  }
  
  if (bAmDecider)
    pQueues->vScheduleAckedTask(pTaskBeingAcked);
  
  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* spawned handler                                                            */
/*----------------------------------------------------------------------------*/
void Handlers::vSpawnedTask(Task* pTask)
{
  //
  // this handler is called from session::bSpawnJob()
  // we want to run given task. task should be ready to run
  //
    
  Queues* pQueues = gpAttachMap->m_pQueues;

  pQueues->vScheduleSpawnedTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* thread loop handler                                                        */
/*----------------------------------------------------------------------------*/
void Handlers::vThreadLoop(Task** ppTask)
{
  //
  // this handler is called from Thread::vLoop()
  // after a task returns to the thread
  //
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  Task* pTask = *ppTask;
  *ppTask = nullptr;
  
  // first remove this task from the running queue
  pQueues->vRemoveFromExecutingSet(pTask);
  
  // now add the task to another appropriate queue depending on its state
  switch (pTask->eState()) {
      
    case TK_EXITED:   pQueues->vScheduleExitedTask(pTask);    break;
    case TK_SEND:     pQueues->vScheduleSendTask(pTask);      break;
    case TK_SLEEPING: pQueues->vScheduleSleepingTask(pTask);  break;
    case TK_WAITING:  pQueues->vScheduleWaitingTask(pTask);   break;
    case TK_YIELDED:  pQueues->vScheduleYieldedTask(pTask);   break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // should never come to thread handler for these states

    case TK_EXECUTING: ASSERT(0);
      
    default:
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
