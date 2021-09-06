/*============================================================================*/
/* Steps_RegTask.cpp - task[] step group                                      */
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
  vAddStep("task[]"_Stz,           &Step::vRegTaskGet);
  
  vAddStep("task[].dropLock"_Stz,  &Step::vTaskDropLock);
  vAddStep("task[].grabLock"_Stz,  &Step::vTaskGrabLock);

  vAddStep("task[].job@"_Stz,      &Step::vTaskJobFetch);
  vAddStep("task[].job!"_Stz,      &Step::vTaskJobStore);
}

/*----------------------------------------------------------------------------*/
/* task[].get -- get task                                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegTaskGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task[].grabLock -- grab this task's lock                                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskGrabLock(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task[].dropLock -- drop this task's lock                                   */
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
