/*============================================================================*/
/* Steps_Prompt.cpp - prompt step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

// prompt: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Prompt(void)
{
  vAddStep("prompt::new"_Stz,             &Task::vPromptNew);
  
  vAddStep("prompt.continue"_Stz,         &Task::vPromptContinue);
  vAddStep("prompt.executeFunction"_Stz,  &Task::vPromptExecuteFunction);
  vAddStep("prompt.getCommand"_Stz,       &Task::vPromptGetCommand);
  vAddStep("prompt.session"_Stz,          &Task::vPromptSession);
}

/*----------------------------------------------------------------------------*/
/* prompt.continue -- get m_bContinue                                         */
/*----------------------------------------------------------------------------*/
void Task::vPromptContinue(MP_Task mpTask)
{
  ENTRY("vPromptContinue"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.continue"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  bool bContinue = pSePrompt->bContinue();
  
  mpTask->m_pDataStack->vPushInt(bContinue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.executeFunction -- execute function                                 */
/*----------------------------------------------------------------------------*/
void Task::vPromptExecuteFunction(MP_Task mpTask)
{
  ENTRY("vPromptExecuteFunction"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.executeCommand"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  bool bFunction = pSePrompt->bExecuteFunction();
  
  mpTask->m_pDataStack->vPushInt(bFunction);
  
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
/* prompt::new -- prompt new                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPromptNew(MP_Task mpTask)
{
  ENTRY("vPromptNew"_Stz);
  
  mpTask->m_pJob->m_pJobClass = JobClass::pFind(CN_JOB_PROMPT);
  
  SePrompt* pSePrompt = new SePrompt;
  
  mpTask->m_pDataStack->vPushObject(pSePrompt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.session -- get se session                                           */
/*----------------------------------------------------------------------------*/
void Task::vPromptSession(MP_Task mpTask)
{
  ENTRY("vPromptSession"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.session"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  Session* pSession = pSePrompt->pSession();
  
  mpTask->m_pDataStack->vPushObject(pSession);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
