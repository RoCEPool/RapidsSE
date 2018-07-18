/*============================================================================*/
/* Steps_Prompt.cpp - prompt step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*      gpAttachMap;

// prompt: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Prompt(void)
{
  vAddPrimitive("prompt.dispatchJob"_Stz,    &vPromptDispatchJob,    false);
  vAddPrimitive("prompt.endJob"_Stz,         &vPromptEndJob,         false);
  vAddPrimitive("prompt.executeCommand"_Stz, &vPromptExecuteCommand, false);
  vAddPrimitive("prompt.getCommand"_Stz,     &vPromptGetCommand,     false);
  vAddPrimitive("prompt.jobClass"_Stz,       &vPromptJobClass,       false);
  vAddPrimitive("prompt::new"_Stz,           &vPromptNew,            false);
  vAddPrimitive("prompt.newJob"_Stz,         &vPromptNewJob,         false);
}

/*----------------------------------------------------------------------------*/
/* prompt.dispatchJob -- prepare and dispatch job                             */
/*----------------------------------------------------------------------------*/
void Task::vPromptDispatchJob(MP_Task mpTask)
{
  ENTRY("vPromptDispatchJob"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.dispatchJob"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  bool b = pSePrompt->bPrepareJob();
  ASSERT(b);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.endJob -- end job                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPromptEndJob(MP_Task mpTask)
{
  ENTRY("vPromptEndJob"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.executeCommand"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  pSePrompt->vEndJob();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.executeCommand -- execute command                                   */
/*----------------------------------------------------------------------------*/
void Task::vPromptExecuteCommand(MP_Task mpTask)
{
  ENTRY("vPromptExecuteCommand"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.executeCommand"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  pSePrompt->vExecuteCommand();
  
  eCK eCommand = pSePrompt->eCommand();
  
  mpTask->m_pDataStack->vPushInt(eCommand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.getCommand -- get next command                                      */
/*----------------------------------------------------------------------------*/
void Task::vPromptGetCommand(MP_Task mpTask)
{
  ENTRY("vPromptGetCommand"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.getCommand"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  pSePrompt->vGetCommand();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.jobClass -- get job class                                           */
/*----------------------------------------------------------------------------*/
void Task::vPromptJobClass(MP_Task mpTask)
{
  ENTRY("vPromptJobClass"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.jobClass"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  JobClass* pJobClass = pSePrompt->pJobClass();
  
  mpTask->m_pDataStack->vPushInt((i8)pJobClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt::new -- prompt new                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPromptNew(MP_Task mpTask)
{
  ENTRY("vPromptNew"_Stz);
  
  SePrompt* pSePrompt = new SePrompt;
  
  mpTask->m_pDataStack->vPushObject(pSePrompt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.newJob -- new a job from job class                                  */
/*----------------------------------------------------------------------------*/
void Task::vPromptNewJob(MP_Task mpTask)
{
  ENTRY("vPromptNewJob"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.newJob"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  pSePrompt->vNewJob();
  
  Job* pJob = pSePrompt->pJob();
  
  pJob->m_pPromptTask = *mpTask;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
