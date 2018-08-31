/*============================================================================*/
/* Steps_Prompt.cpp - prompt step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern JobCatalog gxJobCatalog;

// prompt: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Prompt(void)
{
  vAddPrimitive("prompt.continue"_Stz,         &vPromptContinue,         false);
  vAddPrimitive("prompt.executeFunction"_Stz,  &vPromptExecuteFunction,  false);
  vAddPrimitive("prompt.getCommand"_Stz,       &vPromptGetCommand,       false);
  vAddPrimitive("prompt::new"_Stz,             &vPromptNew,              false);
  vAddPrimitive("prompt.SEitf"_Stz,            &vPromptSEitf,            false);
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
  
  mpTask->m_pJob->m_pJobClass_o = gxJobCatalog.pFindJobClass(CN_JOB_PROMPT);
  
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
  
  SEitf* pSEitf = pSePrompt->pSEitf();
  
  mpTask->m_pDataStack->vPushObject(pSEitf);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
