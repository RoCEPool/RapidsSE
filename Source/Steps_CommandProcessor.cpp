/*============================================================================*/
/* Steps_CommandProcessor.cpp - command processor step group                  */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*      gpAttachMap;

// prompt: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_CommandProcessor(void)
{
  vAddPrimitive("commandProcessor.endJob"_Stz,&vCommandProcessorEndJob,false);
  vAddPrimitive("commandProcessor.newJob"_Stz,&vCommandProcessorNewJob,false);
}

/*----------------------------------------------------------------------------*/
/* prompt.endJob -- end job                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorEndJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorEndJob"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("prompt.executeCommand"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);
  
  pCommandProcessor->vEndJob();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt.newJob -- new a job from job class                                  */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorNewJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorNewJob"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.newJob"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);

  pCommandProcessor->vNewJob();
  
  Job* pJob = pCommandProcessor->m_pJob;
  
  pJob->m_pPromptTask = *mpTask;
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
