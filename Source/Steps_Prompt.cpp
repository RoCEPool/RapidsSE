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
  vAddPrimitive("prompt.executeCommand"_Stz,   &vPromptExecuteCommand,   false);
  vAddPrimitive("prompt.getCommand"_Stz,       &vPromptGetCommand,       false);
  vAddPrimitive("prompt::new"_Stz,             &vPromptNew,              false);
  vAddPrimitive("prompt.SEitf"_Stz,            &vPromptSEitf,            false);
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
  
  eCK eCommand = pSePrompt->m_pCommandProcessor->m_eCommand;
  
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

/*----------------------------------------------------------------------------*/
/* prompt.SEitf -- get se interface                                           */
/*----------------------------------------------------------------------------*/
void Task::vPromptSEitf(MP_Task mpTask)
{
  ENTRY("vPromptSEitf"_Stz);
  
  Object* pSePromptObject;
  if ( ! mpTask->m_pDataStack->bPop(pSePromptObject) )
    THROW_INVALID_DSTACK("prompt.SEitf"_Stz);
  SePrompt* pSePrompt = _pSePrompt(pSePromptObject);
  
  SEitf* pSEitf = pSePrompt->m_pSEitf;
  
  mpTask->m_pDataStack->vPushObject(pSEitf);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
