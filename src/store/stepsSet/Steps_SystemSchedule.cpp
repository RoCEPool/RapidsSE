/*============================================================================*/
/* Steps_Schedule.cpp - System Schedule step group                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Schedule(void)
{
  vAddStep("Schedule::CheckSleeping"_Stz,   &Step::vScheduleCheckSleeping);
  
  vAddStep("Schedule::ExecutingLength"_Stz, &Step::vScheduleExecutingLength);
  vAddStep("Schedule::ReadyLength"_Stz,     &Step::vScheduleReadyLength);
  vAddStep("Schedule::SleepingLength"_Stz,  &Step::vScheduleSleepingLength);
  vAddStep("Schedule::WaitingLength"_Stz,   &Step::vScheduleWaitingLength);
}

/*----------------------------------------------------------------------------*/
/* Schedule::QueuesCheckSleeping -- check the sleeping queue            */
/*----------------------------------------------------------------------------*/
void Step::vScheduleCheckSleeping(void)
{
  ENTRY();
  
  Scheduler::vTaskCross_WakeSleepers(this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Schedule::QueuesExecutingLength -- get the executing queue length    */
/*----------------------------------------------------------------------------*/
void Step::vScheduleExecutingLength(void)
{
  ENTRY();
  
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  Schedule* pSchedule = AM::pSchedule();
  m_pDataStack->vPushInt(pSchedule->nTasksExecuting());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Schedule::QueuesReadyLength -- get the ready queue length            */
/*----------------------------------------------------------------------------*/
void Step::vScheduleReadyLength(void)
{
  ENTRY();
  
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  Schedule* pSchedule = AM::pSchedule();
  m_pDataStack->vPushInt(pSchedule->nTasksReady());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Schedule::QueuesSleepingLength -- get the sleeping queue length      */
/*----------------------------------------------------------------------------*/
void Step::vScheduleSleepingLength(void)
{
  ENTRY();
  
  Schedule* pSchedule = AM::pSchedule();
  m_pDataStack->vPushInt(pSchedule->nTasksSleeping());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Schedule::QueuesWaitingLength -- get the sleeping queue length       */
/*----------------------------------------------------------------------------*/
void Step::vScheduleWaitingLength(void)
{
  ENTRY();
  
  Schedule* pSchedule = AM::pSchedule();
  m_pDataStack->vPushInt(pSchedule->nTasksWaiting());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
