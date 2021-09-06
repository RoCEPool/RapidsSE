/*============================================================================*/
/* Steps_RegTask.cpp - Task[] step group                                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegTask(void)
{
  vAddStep("Task[]"_Stz,              &Step::vRegTaskGet);
  
  vAddStep("Task[].dropLock"_Stz,     &Step::vTaskDropLock);
  vAddStep("Task[].grabLock"_Stz,     &Step::vTaskGrabLock);

  vAddStep("Task[].job@"_Stz,         &Step::vTaskJobFetch);
  vAddStep("Task[].job!"_Stz,         &Step::vTaskJobStore);
  
  vAddStep("Task[].transition!"_Stz,  &Step::vTaskTransitionStore);
  
  vAddStep("Task[].wait"_Stz,         &Step::vTaskWait);
  
  vAddStep("Task[].yield"_Stz,        &Step::vTaskYield);
  vAddStep("Task[].yield_opLoop"_Stz, &Step::vTaskYield_opLoop);
}

/*----------------------------------------------------------------------------*/
/* Task[].get -- get task                                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegTaskGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Task[].grabLock -- grab this task's lock                                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskGrabLock(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Task[].dropLock -- drop this task's lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskDropLock(void)
{
  ENTRY();
    
  vDropTaskLock((Stz*)__func__);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* task.job@ -- push job of given task                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobFetch(void)
{
  ENTRY();
  
//  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
//
// Job* pJob = pThatTask->m_pJob;
//
//  m_pDataStack->vPushPtr(pJob);

  m_pDataStack->vPushPtr(* m_pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job! -- store job into given task                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobStore(void)
{
  ENTRY();
  
//  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
//
//  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
//
//  pThatTask->m_pJob = pJob;
   
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pJob = pJob;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Task[].transition! -- transition store                                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskTransitionStore(void)
{
  ENTRY();
  
  eTR eTransition = (eTR)m_pDataStack->iPopInt(__func__);

  RASSERT(bIsLocked());
  m_eTransition = eTransition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Task[].wait -- this task waits for signal                                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskWait(void)
{
  ENTRY();
  
  vScheduleWait();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Task[].yield  -- this task yields                                            */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);
  vScheduleYield();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Task[].yield  -- this task yields                                            */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield_opLoop(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);
  RASSERT(bIsLocked());
  vScheduleYield();

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
