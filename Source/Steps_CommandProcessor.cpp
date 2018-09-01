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
  vAddPrimitive("commandProcessor.deleteJob"_Stz,   &vCommandProcessorDeleteJob,  false);
  vAddPrimitive("commandProcessor.executeFile"_Stz, &vCommandProcessorExecuteFile,false);
  vAddPrimitive("commandProcessor.findJobClass"_Stz,&vCommandProcessorFindJobClass,false);
  vAddPrimitive("commandProcessor.getCommand"_Stz,  &vCommandProcessorGetCommand, false);
  vAddPrimitive("commandProcessor.jobClass"_Stz,    &vCommandProcessorJobClass,   false);
  vAddPrimitive("commandProcessor.postJob"_Stz,     &vCommandProcessorPostJob,    false);
  vAddPrimitive("commandProcessor.preJob"_Stz,      &vCommandProcessorPreJob,     false);
  vAddPrimitive("commandProcessor.spawnJob"_Stz,    &vCommandProcessorSpawnJob,   false);
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.blankLine -- blank line test                              */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorBlankLine(MP_Task mpTask)
{
  ENTRY("vCommandProcessorBlankLine"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.blankLine"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);
  
  bool bBlank = pCommandProcessor->bBlankLine();
  
  mpTask->m_pDataStack->vPushInt((i8)bBlank);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.deleteJob -- delete job                                   */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorDeleteJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorDeleteJob"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.deleteJob"_Stz);
  CommandProcessor*
  pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);
  
  Object* pSEitfObject;
  if ( ! mpTask->m_pDataStack->bPop(pSEitfObject) )
    THROW_INVALID_DSTACK("commandProcessor.deleteJob"_Stz);
  SEitf*
  pSEitf = _pSEitf(pSEitfObject);
  
  pCommandProcessor->vDeleteJob(pSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CommandProcessor.executeFile -- execute file                               */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorExecuteFile(MP_Task mpTask)
{
  ENTRY("vCommandProcessorExecuteFile"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.postJob"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);

  bool bFile = pCommandProcessor->bExecuteFile();
  
  mpTask->m_pDataStack->vPushInt(bFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CommandProcessor.findJobClass -- find job class                            */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorFindJobClass(MP_Task mpTask)
{
  ENTRY("vCommandProcessorFindJobClass"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.postJob"_Stz);
  CommandProcessor*
  pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);
  
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
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.getCommand"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);
  
  pCommandProcessor->vGetCommand();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.jobClass -- get job class                                 */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorJobClass(MP_Task mpTask)
{
  ENTRY("vCommandProcessorJobClass"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.jobClass"_Stz);
  CommandProcessor*
    pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);

  JobClass* pJobClass = pCommandProcessor->pJobClass();
  
  mpTask->m_pDataStack->vPushInt((i8)pJobClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.postJob -- post process job                               */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorPostJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorPostJob"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.postJob"_Stz);
  CommandProcessor*
  pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);
  
  Object* pSEitfObject;
  if ( ! mpTask->m_pDataStack->bPop(pSEitfObject) )
    THROW_INVALID_DSTACK("commandProcessor.postJob"_Stz);
  SEitf*
  pSEitf = _pSEitf(pSEitfObject);
  
  pCommandProcessor->vPostJob(pSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.preJob -- pre process a job                               */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorPreJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorPreJob"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.preJob"_Stz);
  CommandProcessor*
  pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);
  
  Object* pSEitfObject;
  if ( ! mpTask->m_pDataStack->bPop(pSEitfObject) )
    THROW_INVALID_DSTACK("commandProcessor.postJob"_Stz);
  SEitf* pSEitf = _pSEitf(pSEitfObject);
  
  pCommandProcessor->vPreJob(pSEitf);
  
  Job* pJob = pCommandProcessor->pJob();
  
  pJob->m_pPromptTask = *mpTask;
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* commandProcessor.spawnJob -- spawn job                                     */
/*----------------------------------------------------------------------------*/
void Task::vCommandProcessorSpawnJob(MP_Task mpTask)
{
  ENTRY("vCommandProcessorSpawnJob"_Stz);
  
  Object* pCommandProcessorObj;
  if ( ! mpTask->m_pDataStack->bPop(pCommandProcessorObj) )
    THROW_INVALID_DSTACK("commandProcessor.spawnJob"_Stz);
  CommandProcessor*
  pCommandProcessor = _pCommandProcessor(pCommandProcessorObj);
  
  Object* pSEitfObject;
  if ( ! mpTask->m_pDataStack->bPop(pSEitfObject) )
    THROW_INVALID_DSTACK("commandProcessor.spawnJob"_Stz);
  SEitf*
  pSEitf = _pSEitf(pSEitfObject);
  
  pCommandProcessor->bSpawnJob(pSEitf);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
