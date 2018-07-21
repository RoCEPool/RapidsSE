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
  vAddPrimitive("commandProcessor.endJob"_Stz,  &vCommandProcessorEndJob,  false);
  vAddPrimitive("commandProcessor.jobClass"_Stz,&vCommandProcessorJobClass,false);
  vAddPrimitive("commandProcessor.newJob"_Stz,  &vCommandProcessorNewJob,  false);
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.endJob -- end job                                         */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorEndJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorEndJob"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.endJob"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);
  
  pCommandProcessor->vEndJob();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.jobClass -- get job class                                 */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorJobClass(MP_Task mpTask)
{
  ENTRY("vCommandProcessorJobClass"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.jobClass"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);

  JobClass* pJobClass = pCommandProcessor->m_pJobClass;
  
  mpTask->m_pDataStack->vPushInt((i8)pJobClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.newJob -- new a job from job class                        */
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
