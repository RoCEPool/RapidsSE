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
  vAddPrimitive("commandProcessor.blankLine"_Stz,   &vCommandProcessorBlankLine,  false);
  vAddPrimitive("commandProcessor.dispatchJob"_Stz, &vCommandProcessorDispatchJob,false);
  vAddPrimitive("commandProcessor.endJob"_Stz,      &vCommandProcessorEndJob,     false);
  vAddPrimitive("commandProcessor.executeFile"_Stz, &vCommandProcessorExecuteFile,false);
  vAddPrimitive("commandProcessor.findJobClass"_Stz,&vCommandProcessorFindJobClass,false);
  vAddPrimitive("commandProcessor.getCommand"_Stz,  &vCommandProcessorGetCommand, false);
  vAddPrimitive("commandProcessor.jobClass"_Stz,    &vCommandProcessorJobClass,   false);
  vAddPrimitive("commandProcessor.newJob"_Stz,      &vCommandProcessorNewJob,     false);
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.blankLine -- blank line test                              */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorBlankLine(MP_Task mpTask)
{
  ENTRY("vCommandProcessorBlankLine"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.blankLine"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);
  
  bool bBlank = pCommandProcessor->bBlankLine();
  
  mpTask->m_pDataStack->vPushInt((i8)bBlank);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.dispatchJob -- dispatch job                               */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorDispatchJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorEndJob"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.endJob"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);
  
  Object* pSEitfObject;
  if ( ! mpTask->m_pDataStack->bPop(pSEitfObject) )
    THROW_INVALID_DSTACK("commandProcessor.endJob"_Stz);
  SEitf*
    pSEitf = _pSEitf(pSEitfObject);

  pCommandProcessor->bDispatchJob(pSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CommandProcessor.executeFile -- execute file                               */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorExecuteFile(MP_Task mpTask)
{
  ENTRY("vCommandProcessorExecuteFile"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.endJob"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);

  bool bFile = pCommandProcessor->bExecuteFile();
  
  mpTask->m_pDataStack->vPushInt(bFile);
  
  EXIT();
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
  
  Object* pSEitfObject;
  if ( ! mpTask->m_pDataStack->bPop(pSEitfObject) )
    THROW_INVALID_DSTACK("commandProcessor.endJob"_Stz);
  SEitf*
    pSEitf = _pSEitf(pSEitfObject);
 
  pCommandProcessor->vEndJob(pSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CommandProcessor.findJobClass -- find job class                            */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorFindJobClass(MP_Task mpTask)
{
  ENTRY("vCommandProcessorFindJobClass"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.endJob"_Stz);
  CommandProcessor*
  pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);
  
  bool bFound = pCommandProcessor->bFindJobClass();
  
  mpTask->m_pDataStack->vPushInt(bFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.getCommand -- get command                                 */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorGetCommand(MP_Task mpTask)
{
  ENTRY("vCommandProcessorGetCommand"_Stz);
  
  Object* pCommandProcessorObject;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObject) )
    THROW_INVALID_DSTACK("commandProcessor.getCommand"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObject);
  
  pCommandProcessor->vGetCommand();
  
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

  JobClass* pJobClass = pCommandProcessor->pJobClass();
  
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

  Object* pSEitfObject;
  if ( ! mpTask->m_pDataStack->bPop(pSEitfObject) )
    THROW_INVALID_DSTACK("commandProcessor.endJob"_Stz);
  SEitf*
    pSEitf = _pSEitf(pSEitfObject);
  
  pCommandProcessor->vNewJob(pSEitf);
  
  Job* pJob = pCommandProcessor->pJob();
  
  pJob->m_pPromptTask = *mpTask;
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
