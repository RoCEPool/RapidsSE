/*============================================================================*/
/* Steps_Queues.cpp - queues step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


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
  ENTRY();
  
  Scheduler::vTaskCross_WakeSleepers(this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesExecutingLength -- get the executing queue length            */
/*----------------------------------------------------------------------------*/
void Step::vQueuesExecutingLength(void)
{
  ENTRY();
  
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksExecuting());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesReadyLength -- get the ready queue length                    */
/*----------------------------------------------------------------------------*/
void Step::vQueuesReadyLength(void)
{
  ENTRY();
  
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksReady());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesSleepingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Step::vQueuesSleepingLength(void)
{
  ENTRY();
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksSleeping());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesWaitingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Step::vQueuesWaitingLength(void)
{
  ENTRY();
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksWaiting());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
