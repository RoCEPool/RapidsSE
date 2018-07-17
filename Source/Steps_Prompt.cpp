/*============================================================================*/
/* Steps_Prompt.cpp - prompt step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// prompt: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Prompt(void)
{
  vAddPrimitive("prompt.endJob"_Stz,         &vPromptEndJob,         false);
  vAddPrimitive("prompt.executeCommand"_Stz, &vPromptExecuteCommand, false);
  vAddPrimitive("prompt.getCommand"_Stz,     &vPromptGetCommand,     false);
  vAddPrimitive("prompt::new"_Stz,           &vPromptNew,            false);

  vAddPrimitive("prompt.loop"_Stz,    &vPromptLoop, false);
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
/* prompt.loop -- execute the prompt                                          */
/*----------------------------------------------------------------------------*/
void Task::vPromptLoop(MP_Task mpTask)
{
ENTRY("vPromptLoop"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.execute"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  pSePrompt->vLoop();
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
