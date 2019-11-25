/*============================================================================*/
/* Steps_Queues.cpp - queues step group                                       */
/*============================================================================*/

#include "Steps.h"

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
void Step::vQueuesCheckSleeping(void)
{
  ENTRY(); // ENTRY("vQueuesCheckSleeping"_Stz);
  
  Scheduler::vWakeSleepingTasks(m_pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesExecutingLength -- get the executing queue length            */
/*----------------------------------------------------------------------------*/
void Step::vQueuesExecutingLength(void)
{
  ENTRY(); // ENTRY("vQueuesExecutingLength"_Stz);
  
  i8 iSlot;
  
  if ( !  m_pDataStack->bPopI8(iSlot) )
    THROW_DSTACK();
  
  Queues* pQueues = AM::pQueues();
  m_pDataStack->vPushInt(pQueues->nTasksExecuting());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesReadyLength -- get the ready queue length                    */
/*----------------------------------------------------------------------------*/
void Step::vQueuesReadyLength(void)
{
  ENTRY(); // ENTRY("vQueuesReadyLength"_Stz);
  
  i8 iSlot;
  
  if ( !  m_pDataStack->bPopI8(iSlot) )
    THROW_DSTACK();
  
  Queues* pQueues = AM::pQueues();
  m_pDataStack->vPushInt(pQueues->nTasksReady());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesSleepingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Step::vQueuesSleepingLength(void)
{
  ENTRY(); // ENTRY("vQueuesSleepingLength"_Stz);
  
  Queues* pQueues = AM::pQueues();
  m_pDataStack->vPushInt(pQueues->nTasksSleeping());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesWaitingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Step::vQueuesWaitingLength(void)
{
  ENTRY(); // ENTRY("vQueuesWaitingLength"_Stz);
  
  Queues* pQueues = AM::pQueues();
  m_pDataStack->vPushInt(pQueues->nTasksWaiting());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
