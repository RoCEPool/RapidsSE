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
  vAddPrimitive("prompt.commandProcessor"_Stz, &vPromptCommandProcessor, false);
  vAddPrimitive("prompt.dispatchJob"_Stz,    &vPromptDispatchJob,    false);
  vAddPrimitive("prompt.executeCommand"_Stz, &vPromptExecuteCommand, false);
  vAddPrimitive("prompt.getCommand"_Stz,     &vPromptGetCommand,     false);
  vAddPrimitive("prompt::new"_Stz,           &vPromptNew,            false);
}

/*----------------------------------------------------------------------------*/
/* prompt.commandProcessor -- get command processor                           */
/*----------------------------------------------------------------------------*/
void Task::vPromptCommandProcessor(MP_Task mpTask)
{
  ENTRY("vPromptCommandProcessor"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.commandProcessor"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  CommandProcessor* pCommandProcessor =
    pSePrompt->m_pCommandProcessor;
  
  mpTask->m_pDataStack->vPushObject(pCommandProcessor);
  
  EXIT();
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
  
  pSePrompt->m_pCommandProcessor->vGetCommand();
  
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
