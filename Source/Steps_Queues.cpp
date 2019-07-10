/*============================================================================*/
/* Steps_Queues.cpp - queues step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Queues(void)
{
  vAddStep("queues::CheckSleeping"_Stz,   &Step::vQueuesCheckSleeping);
  
  vAddStep("queues::ExecutingLength"_Stz, &Step::vQueuesExecutingLength);
  vAddStep("queues::ReadyLength"_Stz,     &Step::vQueuesReadyLength);
  vAddStep("queues::SleepingLength"_Stz,  &Step::vQueuesSleepingLength);
  vAddStep("queues::WaitingLength"_Stz,   &Step::vQueuesWaitingLength);
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesCheckSleeping -- check the sleeping queue                    */
/*----------------------------------------------------------------------------*/
void Step::vQueuesCheckSleeping(MP_Task mpTask)
{
  ENTRY("vQueuesCheckSleeping"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesExecutingLength -- get the executing queue length            */
/*----------------------------------------------------------------------------*/
void Step::vQueuesExecutingLength(MP_Task mpTask)
{
  ENTRY("vQueuesExecutingLength"_Stz);
  
  i8 iSlot;
  
  if ( !  mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("queues::QueuesExecutingLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushI8(pQueues->nTasksExecuting());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesReadyLength -- get the ready queue length                    */
/*----------------------------------------------------------------------------*/
void Step::vQueuesReadyLength(MP_Task mpTask)
{
  ENTRY("vQueuesReadyLength"_Stz);
  
  i8 iSlot;
  
  if ( !  mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("queues::QueuesReadyLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushI8(pQueues->nTasksReady());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesSleepingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Step::vQueuesSleepingLength(MP_Task mpTask)
{
  ENTRY("vQueuesSleepingLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushI8(pQueues->nTasksSleeping());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesWaitingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Step::vQueuesWaitingLength(MP_Task mpTask)
{
  ENTRY("vQueuesWaitingLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushI8(pQueues->nTasksWaiting());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
