/*============================================================================*/
/* Steps_SystemSchedule.cpp - System Schedule step group                      */
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
void Step::vAddSteps_SystemSchedule(void)
{
  vAddStep("SystemSchedule::CheckSleeping"_Stz,   &Step::vSystemScheduleCheckSleeping);
  
  vAddStep("SystemSchedule::ExecutingLength"_Stz, &Step::vSystemScheduleExecutingLength);
  vAddStep("SystemSchedule::ReadyLength"_Stz,     &Step::vSystemScheduleReadyLength);
  vAddStep("SystemSchedule::SleepingLength"_Stz,  &Step::vSystemScheduleSleepingLength);
  vAddStep("SystemSchedule::WaitingLength"_Stz,   &Step::vSystemScheduleWaitingLength);
}

/*----------------------------------------------------------------------------*/
/* SystemSchedule::QueuesCheckSleeping -- check the sleeping queue            */
/*----------------------------------------------------------------------------*/
void Step::vSystemScheduleCheckSleeping(void)
{
  ENTRY();
  
  Scheduler::vTaskCross_WakeSleepers(this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SystemSchedule::QueuesExecutingLength -- get the executing queue length    */
/*----------------------------------------------------------------------------*/
void Step::vSystemScheduleExecutingLength(void)
{
  ENTRY();
  
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksExecuting());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SystemSchedule::QueuesReadyLength -- get the ready queue length            */
/*----------------------------------------------------------------------------*/
void Step::vSystemScheduleReadyLength(void)
{
  ENTRY();
  
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksReady());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SystemSchedule::QueuesSleepingLength -- get the sleeping queue length      */
/*----------------------------------------------------------------------------*/
void Step::vSystemScheduleSleepingLength(void)
{
  ENTRY();
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksSleeping());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SystemSchedule::QueuesWaitingLength -- get the sleeping queue length       */
/*----------------------------------------------------------------------------*/
void Step::vSystemScheduleWaitingLength(void)
{
  ENTRY();
  
  SystemSchedule* pSystemSchedule = AM::pSystemSchedule();
  m_pDataStack->vPushInt(pSystemSchedule->nTasksWaiting());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
