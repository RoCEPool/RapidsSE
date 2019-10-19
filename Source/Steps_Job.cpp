/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Job(void)
{
  vAddStep("job::this"_Stz,            &Step::vJobThis);
  vAddStep("job::completed"_Stz,       &Step::vJobCompleted);

  vAddStep("job.acksRemaining!"_Stz,   &Step::vJobAcksRemaining);
  vAddStep("job.addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("job.addParticipant"_Stz,   &Step::vJobAddParticipant);
  vAddStep("job.assertBegin"_Stz,      &Step::vJobAssertBegin);
  vAddStep("job.assertNeutral"_Stz,    &Step::vJobAssertNeutral);
  vAddStep("job.assertOriginator"_Stz, &Step::vJobAssertOriginator);
  vAddStep("job.assertParticipant"_Stz,&Step::vJobAssertParticipant);
  vAddStep("job.assertReceived"_Stz,   &Step::vJobAssertReceived);

  vAddStep("job.catalogName@"_Stz,     &Step::vJobCatalogNameFetch);
  vAddStep("job.class"_Stz,            &Step::vJobClass);
  vAddStep("job.columnCount@"_Stz,     &Step::vJobColumnCountFetch);
  vAddStep("job.columnIsPartKey"_Stz,  &Step::vJobColumnIsPartKey);
  vAddStep("job.columnName@"_Stz,      &Step::vJobColumnNameFetch);
  vAddStep("job.columnType@"_Stz,      &Step::vJobColumnTypeFetch);
  vAddStep("job.createResponseVec"_Stz,&Step::vJobCreateResponseVec);
  
  vAddStep("job.decAcksRemaining"_Stz, &Step::vJobDecAcksRemaining);
  vAddStep("job.delete"_Stz,           &Step::vJobDelete);
  vAddStep("job.dicPath"_Stz,          &Step::vJobDicPath);
  
  vAddStep("job.ename!"_Stz,           &Step::vJobEnameStore);
  vAddStep("job.ename@"_Stz,           &Step::vJobEnameFetch);
  vAddStep("job.error!"_Stz,           &Step::vJobErrorStore);
  vAddStep("job.error@"_Stz,           &Step::vJobErrorFetch);
  vAddStep("job.execute@"_Stz,         &Step::vJobExecuteFetch);

  vAddStep("job::findOriginator"_Stz,  &Step::vJobFindOriginator);
  vAddStep("job::findParticipant"_Stz, &Step::vJobFindParticipant);
  vAddStep("job.folderName@"_Stz,      &Step::vJobFolderNameFetch);

  vAddStep("job.hasError"_Stz,         &Step::vJobHasError);
  vAddStep("job.hasOperand"_Stz,       &Step::vJobHasOperand);

  vAddStep("job.id@"_Stz,              &Step::vJobIdFetch);
  vAddStep("job.indexName@"_Stz,       &Step::vJobIndexNameFetch);
  vAddStep("job.IsExecuting"_Stz,      &Step::vJobIsExecuting);
  
  vAddStep("job.jobClass"_Stz,         &Step::vJobJobClass);
  vAddStep("job.jobList@"_Stz,         &Step::vJobJobListFetch);
  
  vAddStep("job.metadata!"_Stz,        &Step::vJobMetadataStore);
  
  vAddStep("job.nodeName@"_Stz,        &Step::vJobNodeNameFetch);
  vAddStep("job.noWait@"_Stz,          &Step::vJobNoWaitFetch);
  
  vAddStep("job.objectName@"_Stz,      &Step::vJobObjectNameFetch);
  
  vAddStep("job.operand@"_Stz,         &Step::vJobOperandFetch);
  vAddStep("job.operand!"_Stz,         &Step::vJobOperandStore);
  
  vAddStep("job.originatingTask@"_Stz, &Step::vJobOriginatingTaskFetch);
  vAddStep("job.originatingTask!"_Stz, &Step::vJobOriginatingTaskStore);
  vAddStep("job::originator@"_Stz,     &Step::vJobOriginatorFetch);

  vAddStep("job.parentId@"_Stz,        &Step::vJobParentIdFetch);
  vAddStep("job.participant@"_Stz,     &Step::vJobParticipantFetch);
  vAddStep("job.participant!"_Stz,     &Step::vJobParticipantStore);
  vAddStep("job.participantResponse!"_Stz, &Step::vJobParticipantResponseStore);
  vAddStep("job.participantHasError"_Stz,  &Step::vJobParticipantHasError);
  vAddStep("job.partitions@"_Stz,      &Step::vJobPartitionsFetch);
  vAddStep("job.phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("job.phase!"_Stz,           &Step::vJobPhaseStore);
  vAddStep("job.postRecv"_Stz,         &Step::vJobPostRecv);
  vAddStep("job.preSend"_Stz,          &Step::vJobPreSend);

  vAddStep("job.recv"_Stz,             &Step::vJobRecv);
  vAddStep("job.regionSize@"_Stz,      &Step::vJobRegionSizeFetch);
  vAddStep("job.removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("job.removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("job.reseed"_Stz,           &Step::vJobReseed);
  vAddStep("job.response@"_Stz,        &Step::vJobResponseFetch);
  vAddStep("job.response!"_Stz,        &Step::vJobResponseStore);
  vAddStep("job.routineName@"_Stz,     &Step::vJobRoutineNameFetch);
  vAddStep("job.rowsInvalid@"_Stz,     &Step::vJobRowsInvalidFetch);
  vAddStep("job.rowsInvalid!"_Stz,     &Step::vJobRowsInvalidStore);
  vAddStep("job.rowsProcessed@"_Stz,   &Step::vJobRowsProcessedFetch);
  vAddStep("job.rowsProcessed!"_Stz,   &Step::vJobRowsProcessedStore);
  vAddStep("job.rowsSkipped@"_Stz,     &Step::vJobRowsSkippedFetch);
  vAddStep("job.rowsSkipped!"_Stz,     &Step::vJobRowsSkippedStore);

  vAddStep("job.schemaName@"_Stz,      &Step::vJobSchemaNameFetch);
  vAddStep("job.seed@"_Stz,            &Step::vJobSeedFetch);
  vAddStep("job.send"_Stz,             &Step::vJobSend);
  vAddStep("job.sessionState@"_Stz,    &Step::vJobSessionStateFetch);
  vAddStep("job.show"_Stz,             &Step::vJobShow);
  vAddStep("job.signalWaiter"_Stz,     &Step::vJobSignalWaiter);
  vAddStep("job.slot@"_Stz,            &Step::vJobSlotFetch);
  vAddStep("job.spawn"_Stz,            &Step::vJobSpawn);
  vAddStep("job.spawn_dedicated"_Stz,  &Step::vJobSpawnDedicated);
  vAddStep("job::stateDiagram"_Stz,    &Step::vJobStateDiagram);

  vAddStep("job.task@"_Stz,            &Step::vJobTaskFetch);
  vAddStep("job.task!"_Stz,            &Step::vJobTaskStore);
  vAddStep("job.tenantName@"_Stz,      &Step::vJobTenantNameFetch);
 
  vAddStep("job::waitParticipant"_Stz, &Step::vJobWaitParticipant);
}


/*----------------------------------------------------------------------------*/
/* job.acksRemaining! -- set acks remaining for this job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobAcksRemaining(Forth* pForth)
{
  ENTRY("vJobAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 nAcksRemaining;
  if ( ! pForth->m_pDataStack->bPop(nAcksRemaining) )
    THROW_DSTACK();
  
  pJob->vAcksRmg(nAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addOriginator -- add job to originator job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobAddOriginator(Forth* pForth)
{
  ENTRY("vJobAddOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
    
  // add the job to the originating jobs
//  AM::pOriginatingJobs()->vGrabLock();
//  AM::pOriginatingJobs()->vAddJob(pJob);
//  AM::pOriginatingJobs()->vDropLock();
  
  Scheduler::vAddOriginatingJob(pJob);

  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Added to Originator List: %s %p"_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    AM::pOriginatingJobs()->vLogJobs(pJob->m_iUniqueId);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addParticipant -- add job to participant job set                       */
/*----------------------------------------------------------------------------*/
void Step::vJobAddParticipant(Forth* pForth)
{
  ENTRY("vJobAddParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
//  AM::pParticipatingJobs()->vGrabLock();
//  AM::pParticipatingJobs()->vAddJob(pJob);
//  AM::pParticipatingJobs()->vDropLock();
  
  Scheduler::vAddParticipatingJob(pJob);

  //vTrace("%p ID %lld added to participant\n"_Stz, pJob, pJob->iJobId());
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Added to ParticipantList: %s %p "_Stz,
                          aCN[pJob->eClass()],
                          pJob);
    AM::pParticipatingJobs()->vLogJobs(pJob->m_iUniqueId);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertBegin -- assert job is not in originator nor participant set     */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertBegin(Forth* pForth)
{
  ENTRY("vJobAssertBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (Job::iNodeNo(pJob->m_iUniqueId) == AM::pGrid()->m_iThisNode) {
    
    if (! pJob->m_bWasReceived)
      vFatal("job.assertOriginator: "
             "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    if (JL_NONE != pJob->m_eJobList)
      vFatal("job.assertBegin: "
             "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iUniqueId);
    if (! pJobFound)
      vFatal("job.assertBegin: "
             "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iUniqueId);
    if (pJobFound)
      vFatal("job.assertBegin: "
             "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pJob->m_bWasReceived)
    vFatal("job.assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  if (JL_NONE != pJob->m_eJobList)
    vFatal("job.assertBegin: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iUniqueId);
  if (pJobFound)
    vFatal("job.assertBegin: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iUniqueId);
  if (pJobFound)
    vFatal("job.assertBegin: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertNeutral -- assert job is not in originator nor participant set   */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertNeutral(Forth* pForth)
{
  ENTRY("vJobAssertNeutral"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (JL_NONE != pJob->m_eJobList)
    vFatal("job.assertNeutral: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iUniqueId);
  if (pJobFound)
    vFatal("job.assertNeutral: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iUniqueId);
  if (pJobFound)
    vFatal("job.assertNeutral: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertOriginator -- assert job is in originator job set                */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertOriginator(Forth* pForth)
{
  ENTRY("vJobAssertOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (JL_ORIGINATING != pJob->m_eJobList)
    vFatal("job.assertOriginator: "
           "Job::m_eJobList is not JL_ORIGINATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  if (pJob->m_bWasReceived)
    vFatal("job.assertOriginator: "
           "Job::m_bWasReceived is true [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iUniqueId);
  if (! pJobFound)
    vFatal("job.assertOriginator: "
           "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertParticipant -- assert job is in participant job set              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertParticipant(Forth* pForth)
{
  ENTRY("vJobAssertParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (JL_PARTICIPATING != pJob->m_eJobList)
    vFatal("job.assertParticipant: "
           "Job::m_eJobList is not JL_PARTICIPATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  if (! pJob->m_bWasReceived)
    vFatal("job.assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iUniqueId);
  if (! pJobFound)
    vFatal("job.assertParticipant: "
           "Job not found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertReceived -- assert job was received                              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertReceived(Forth* pForth)
{
  ENTRY("vJobAssertReceived"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (! pJob->m_bWasReceived)
    vFatal("job.assertReceived: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.catalogName@ -- fetch the catalog name of the job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobCatalogNameFetch(Forth* pForth)
{
  ENTRY("vJobCatalogNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sCatalogName;
  
  sCatalogName = pJob->szCatalogName();
  
  pForth->m_pDataStack->vPushS32(sCatalogName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClass(Forth* pForth)
{
  ENTRY("vJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  eCN eClass = pJob->eClass();
  
  pForth->m_pDataStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnCount@ -- fetch the no of columns of the create table job        */
/*----------------------------------------------------------------------------*/
void Step::vJobColumnCountFetch(Forth* pForth)
{
  ENTRY("vJobColumnCountFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob_CreateTable->m_iColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnIsPartKey -- return true if column is a partition by key        */
/*----------------------------------------------------------------------------*/
void Step::vJobColumnIsPartKey(Forth* pForth)
{
  ENTRY("vJobColumnIsPartKey"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! pForth->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushI8(pJob_CreateTable->m_xColumns[iColumnNo].bIsPtnKey());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnName@ -- fetch the name of the column of the create table job    */
/*----------------------------------------------------------------------------*/
void Step::vJobColumnNameFetch(Forth* pForth)
{
  ENTRY("vJobColumnNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! pForth->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushS32(pJob_CreateTable->m_xColumns[iColumnNo].sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnType@ -- fetch column type from create table job                 */
/*----------------------------------------------------------------------------*/
void Step::vJobColumnTypeFetch(Forth* pForth)
{
  ENTRY("vJobColumnTypeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! pForth->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushI8(pJob_CreateTable->m_xColumns[iColumnNo].eColumnType());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.completed --                                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobCompleted(Forth* pForth)
{
  ENTRY("vJobCompleted"_Stz);
  
  Job* pJob = pForth->m_pTask->pJob();
  
  pJob->m_eJobState = JS_COMPLETED;
  
  pForth->m_pTask->vTransition(TR_DELETE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.createResponseVec -- create the response vector in the job             */
/*----------------------------------------------------------------------------*/
void Step::vJobCreateResponseVec(Forth* pForth)
{
  ENTRY("vJobCreateResponseVec"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  RASSERT( ! pJob->m_pResponseVec );
  pJob->m_pResponseVec = new ResponseVec;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.operand@ -- fetch operand from job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobOperandFetch(Forth* pForth)
{
  ENTRY("vJobOperandFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushPointer(pJob->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.operand! -- store operand in the job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobOperandStore(Forth* pForth)
{
  ENTRY("vJobOperandStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  Object* pOperandPtr;
  if ( ! pForth->m_pDataStack->bPop(pOperandPtr) )
    THROW_DSTACK();
  Operand* pOperand = _pOperand(pOperandPtr);
  
  pJob->m_pOperand = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.delete -- delete the job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobDelete(Forth* pForth)
{
  ENTRY("vJobDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  //vTrace("Deleting job %p\n"_Stz, pJob);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  // call the job delete function
  pJobClass->vDeleteJob(pJob);
  
  EXIT();
}
  

/*----------------------------------------------------------------------------*/
/* job.decAcksRemaining -- decrement acks remaining for this job              */
/*----------------------------------------------------------------------------*/
void Step::vJobDecAcksRemaining(Forth* pForth)
{
  ENTRY("vJobDecAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  bool bAmDecider = pJob->m_oAcksRmg.bDecAndDecide();
  
  pForth->m_pDataStack->vPushI8(bAmDecider);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.dicPath -- get job dic path                                            */
/*----------------------------------------------------------------------------*/
void Step::vJobDicPath(Forth* pForth)
{
  ENTRY("vJobDicPath"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);

  pForth->m_pDataStack->vPushPointer(&pJob->m_oDicPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.ename@ -- fetch/decode given ename from job EnameSet                     */
/*----------------------------------------------------------------------------*/
void Step::vJobEnameFetch(Forth* pForth)
{
  ENTRY("vJobEnameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sEname;
  if ( ! pForth->m_pDataStack->bPop(sEname) )
    THROW_DSTACK();
  
  s4K sValue ( pJob->m_oEnmSet.szValue(sEname) );
  
  pForth->m_pDataStack->vPushS4K(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.ename! -- store given ename-value into job EnameSet                      */
/*----------------------------------------------------------------------------*/
void Step::vJobEnameStore(Forth* pForth)
{
  ENTRY("vJobEnameStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sEname;
  if ( ! pForth->m_pDataStack->bPop(sEname) )
    THROW_DSTACK();
  
  s4K sValue;
  if ( ! pForth->m_pDataStack->bPop(sValue) )
    THROW_DSTACK();

  RASSERT(0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error@ -- fetch the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorFetch(Forth* pForth)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);

  i8 iNode;
  if ( ! pForth->m_pDataStack->bPopAsInt(iNode) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushI8(pJob->m_xResponse.m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error! -- store the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorStore(Forth* pForth)
{
  ENTRY("vJobErrorStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobError;
  if ( ! pForth->m_pDataStack->bPopAsInt(iJobError) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_eErrorCode = (eEC)iJobError;
  pJob->m_bHasError = true;
  pJob->m_xResponse.m_eErrorCode = (eEC)iJobError;
  
  //ActLog::vAdd(pJob,AN_ERROR_CODE,iJobError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.execute@ -- fetch the job word to be executed                          */
/*----------------------------------------------------------------------------*/
void Step::vJobExecuteFetch(Forth* pForth)
{
  ENTRY("vJobExecuteFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sExecute;

  switch (pJob->eClass()) {
      
    case CN_JOB__BEGUN:
    case CN_JOB__CANCEL:
    case CN_JOB__CANCELLED:
    case CN_JOB__END:
    case CN_JOB__ENDED:
    case CN_JOB__EXECUTED:
    case CN_JOB__EXECUTE:
      ASSERT(0);
      break;
      
    default:
      StrLib::vCopy(sExecute.azChar(),pJob->m_aJobClass->m_aSeedExe->azChar());
      //sExecute.vConcat("_exe"_Stz);
      break;
  }
  
  pForth->m_pDataStack->vPushS32(sExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findOriginator -- find job from originator job set                    */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(Forth* pForth)
{
  ENTRY("vJobFindOriginator"_Stz);
  
  i8 iJobId;
  if ( ! pForth->m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  Job* pJobFound = Scheduler::pFindOriginatingJob(iJobId);
  RASSERT(pJobFound);
  
  pForth->m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findParticipant -- find job from participant job set                  */
/*----------------------------------------------------------------------------*/
void Step::vJobFindParticipant(Forth* pForth)
{
  ENTRY("vJobFindParticipant"_Stz);
  
  i8 iJobId;
  if ( ! pForth->m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
    
  Job* pJobFound = Scheduler::pFindParticipatingJob(iJobId);
  RASSERT(pJobFound);
  
  pForth->m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.folderName@ -- fetch the folder name of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobFolderNameFetch(Forth* pForth)
{
  ENTRY("vJobFolderNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sFolderName;
  
  sFolderName = pJob->szFolderName();
  
  pForth->m_pDataStack->vPushS32(sFolderName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasError -- return true if job has error                               */
/*----------------------------------------------------------------------------*/
void Step::vJobHasError(Forth* pForth)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_bHasError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasOperand -- return true if job has an operand                        */
/*----------------------------------------------------------------------------*/
void Step::vJobHasOperand(Forth* pForth)
{
  ENTRY("vJobHasOperand"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  
  i8 bHas = pJob->m_pOperand ? true : false;
  if (pJob->m_aJobClass->m_eClass == CN_JOB_WRITEBUFFER)
    int iDebug = 1;
  
  pForth->m_pDataStack->vPushI8(bHas);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch(Forth* pForth)
{
  ENTRY("vJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_iUniqueId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.indexName@ -- fetch the index name of the job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobIndexNameFetch(Forth* pForth)
{
  ENTRY("vJobIndexNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);

  s32 sIndexName = pJob->szIndexName();
  pForth->m_pDataStack->vPushS32(sIndexName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.isExecuting -- is the job executing (SEED_EXE)                         */
/*----------------------------------------------------------------------------*/
void Step::vJobIsExecuting(Forth* pForth)
{
  ENTRY("vJobIsExecuting"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iIsExecuting = pJob->m_eSeed == SS_SEED_EXE ? 1 : 0;
  pForth->m_pDataStack->vPushI8(iIsExecuting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobClass -- get the JobClass of the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobJobClass(Forth* pForth)
{
  ENTRY("vJobJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobClass_p = (i8)pJob->eClass();
  
  pForth->m_pDataStack->vPushI8(iJobClass_p);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobList@ -- fetch job list                                             */
/*----------------------------------------------------------------------------*/
void Step::vJobJobListFetch(Forth* pForth)
{
  ENTRY("vJobJobListFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_eJobList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.metadata! -- store meta data string in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJobMetadataStore(Forth* pForth)
{
  ENTRY("vJobMetadataStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s4K sMetadata;
  if ( ! pForth->m_pDataStack->bPop(sMetadata) )
    THROW_DSTACK();
  
  pJob->m_sMetadata = sMetadata;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.nodeName@ -- fetch the node name of the job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobNodeNameFetch(Forth* pForth)
{
  ENTRY("vJobNodeNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sNodeName;
  
  sNodeName = pJob->szNodeName();
  
  pForth->m_pDataStack->vPushS32(sNodeName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.noWait@ -- fetch no-wait                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobNoWaitFetch(Forth* pForth)
{
  ENTRY("vJobNoWaitFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_bNoWait);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.objectName@ -- fetch the object name of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobObjectNameFetch(Forth* pForth)
{
  ENTRY("vJobObjectNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);

  s32 sObjectName;
  
  sObjectName = pJob->szObjectName();
  
  pForth->m_pDataStack->vPushS32(sObjectName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask@ -- fetch the originating task from the job            */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskFetch(Forth* pForth)
{
  ENTRY("vJobOriginatingTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushPointer(pJob->m_pOriginatingTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask! -- store the originating task in the job              */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskStore(Forth* pForth)
{
  ENTRY("vJobOriginatingTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  Object* pOriginatingTaskPtr;
  if ( ! pForth->m_pDataStack->bPop(pOriginatingTaskPtr) )
    THROW_DSTACK();
  Task* pOriginatingTask = _pTask(pOriginatingTaskPtr);
  ASSERT(CN_TASK == pOriginatingTask->eClass());
  
  pJob->m_pOriginatingTask = pOriginatingTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::originator@ -- fetch the originator node from the job                  */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatorFetch(Forth* pForth)
{
  ENTRY("vJobOriginatorFetch"_Stz);
  
  i8 iJobId;
  if ( ! pForth->m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushI8(Job::iOriginatorNode(iJobId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.parentId@ -- fetch job parent id                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobParentIdFetch(Forth* pForth)
{
  ENTRY("vJobParentIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_iParentId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participant@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantFetch(Forth* pForth)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_iParticipantNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partcipant! -- store the participant node in the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantStore(Forth* pForth)
{
  ENTRY("vJobParticipantStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iParticipantNode;
  if ( ! pForth->m_pDataStack->bPopAsInt(iParticipantNode) )
    THROW_DSTACK();
  
  pJob->m_iParticipantNode = iParticipantNode;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantHasError -- has any participant returned an error ?         */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantHasError(Forth* pForth)
{
  ENTRY("job.participantHasError"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 bHasError = false;
  i8 iResponses = pJob->m_pResponseVec->nItems();
  
  for (i8 i_Response = 0; i_Response < iResponses; i_Response++) {
    if (pJob->m_pResponseVec->pGet(i_Response)->m_eErrorCode != EC_NONE) {
      bHasError = true;
      break;
    }
  }
  pForth->m_pDataStack->vPushI8(bHasError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantResponse! -- store the job response for the partcipant node */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantResponseStore(Forth* pForth)
{
  ENTRY("vJobParticipantResponseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! pForth->m_pDataStack->bPopAsInt(iNode) )
    THROW_DSTACK();
  
  Object* pParticipantJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pParticipantJobPtr) )
    THROW_DSTACK();
  Job* pParticipantJob = _pJob(pParticipantJobPtr);
  
  // there should be a response vector
  RASSERT( pJob->m_pResponseVec );
  
  // create a new response
  Response* pResponse = new Response;
  
  // set node and error in the response
  pResponse->m_iNode           = iNode;
  pResponse->m_sResponse       = pParticipantJob->m_xResponse.m_sResponse;
  pResponse->m_ixRowsProcessed = pParticipantJob->m_xResponse.m_ixRowsProcessed;
  pResponse->m_ixRowsInvalid   = pParticipantJob->m_xResponse.m_ixRowsInvalid;
  pResponse->m_ixRowsSkipped   = pParticipantJob->m_xResponse.m_ixRowsSkipped;
  pResponse->m_eErrorCode      = pParticipantJob->m_xResponse.m_eErrorCode;
  
  if (pResponse->m_eErrorCode != EC_NONE)
    pJob->m_bHasError = true;
  
  // add the response to the vector of responses
  pJob->m_pResponseVec->vAdd(pResponse);
  
  if (pResponse->m_eErrorCode != EC_NONE)
    //ActLog::vAdd(pJob,AN_ERROR_CODE,pResponse->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partitions@ -- fetch partitions from job_sanity                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPartitionsFetch(Forth* pForth)
{
  ENTRY("vJobPartitionsFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob_Sanity->m_iPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase@ -- fetch the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(Forth* pForth)
{
  ENTRY("vJobPhaseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_eJobPhase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase! -- store the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseStore(Forth* pForth)
{
  ENTRY("vJobPhaseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobPhase;
  if ( ! pForth->m_pDataStack->bPopAsInt(iJobPhase) )
    THROW_DSTACK();
  
  pJob->m_eJobPhase = (eJP)iJobPhase;
  
  //ActLog::vAdd(pJob,AN_JOB_PHASE,iJobPhase,_EG_JP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.postRecv -- run the post-receive function of the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobPostRecv(Forth* pForth)
{
  ENTRY("vJobPostRecv"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the job class on this node
  
  JobClass* aJobClass = nullptr;
  eCN eClass = pJob->eClass();
  
  if (CN_JOB_UNIVERSAL == eClass) {
    eClass = (eCN)pJob->m_oEnmSet.iValue("<jobClass>"_Stz);
    aJobClass = JobClass::pFind(eClass);
  }
  else {
    aJobClass = JobClass::pFind(eClass);
  }
  
  pJob->m_aJobClass = aJobClass;

  ASSERT(aJobClass->m_fpRecv == vJob_recv);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // run the post recv function
  
  if (aJobClass->m_fpRecv)
    (*aJobClass->m_fpRecv)(pJob);
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,
                          "PostRecv: %s %p"_Stz,
                          aCN[pJob->eClass()],
                          pJob);
    pJob->vMarkJob();
  }
  
  //ActLog::vAdd(pJob,AN_POST_RECV,pForth->m_pTask->pJob());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.preSend -- run the pre-send function of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPreSend(Forth* pForth)
{
  ENTRY("vJobPreSend"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  JobClass* pJobClass = pJob->m_aJobClass;
  
  //ActLog::vAdd(pJob,AN_PRE_SEND,pForth->m_pTask->pJob());
  if (pJobClass->m_fpSend)
    (*pJobClass->m_fpSend)(pJob);
  
  if (pJob->m_xReqTuple.m_iDetail)
    JobLog::vNote(pJob,
                          "PreSend: %s %p"_Stz,
                          aCN[pJob->eClass()],
                          pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.recv -- post-receive function                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobRecv(Forth* pForth)
{
  ENTRY("vJobRecv"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  // call the job post-receive function
  pJobClass->vRecvJob(pJob);  // come back to this, remove session as parameter to delete
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeOriginator -- remove job from originator job set                 */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveOriginator(Forth* pForth)
{
  ENTRY("vJobRemoveOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  bool bRemove = Scheduler::bRemoveOriginatingJob(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from originator\n"_Stz,
           aCN[pJob->eClass()],pJob->m_iUniqueId);
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = Scheduler::bRemoveOriginatingJob(pJob,true);
  }
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,
                       "Removed from Originator List: %s %p"_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    AM::pOriginatingJobs()->vLogJobs(pJob->m_iUniqueId);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeParticipant -- remove job from participant job set               */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveParticipant(Forth* pForth)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  bool bRemove = Scheduler::bRemoveParticipatingJob(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from participant\n"_Stz,
           aCN[pJob->eClass()],pJob->m_iUniqueId);
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = Scheduler::bRemoveParticipatingJob(pJob,true);
  }
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Removed from Participant List: "_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    AM::pParticipatingJobs()->vLogJobs(pJob->m_iUniqueId);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.regionSizeResponse@ -- fetch region size from job_sanity               */
/*----------------------------------------------------------------------------*/
void Step::vJobRegionSizeFetch(Forth* pForth)
{
  ENTRY("vJobRegionSizeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob_Sanity->m_iRegionSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.reseed -- reseed the job on the stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobReseed(Forth* pForth)
{
  ENTRY("vJobReseed"_Stz);
  
  i8 i_Seed;
  if ( ! pForth->m_pDataStack->bPop(i_Seed) )
    THROW_DSTACK();
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  AM::vLogSvc(SL_INF,pForth->m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[pForth->m_pTask->m_pJob->eClass()],
              C_ENDARGS);
  
  pJob->vSeed((eSS)i_Seed);
  IGNORE_RETURN(Api::bReseedJob(nullptr,pJob));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response@ -- fetch the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseFetch(Forth* pForth)
{
  ENTRY("vJobResponseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushS4K(pJob->m_xResponse.m_sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response! -- store the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseStore(Forth* pForth)
{
  ENTRY("vJobResponseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s4K sResponse;
  if ( ! pForth->m_pDataStack->bPop(sResponse) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_sResponse = sResponse;
  pJob->m_xResponse.m_sResponse = sResponse;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.routineName@ -- fetch the routine name of the job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobRoutineNameFetch(Forth* pForth)
{
  ENTRY("vJobRoutineNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sRoutineName;
  
  sRoutineName = pJob->szRoutineName();
  
  pForth->m_pDataStack->vPushS32(sRoutineName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid@ -- fetch the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidFetch(Forth* pForth)
{
  ENTRY("vJobRowsInvalidFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsInvalid = *pJob->m_xResponse.m_ixRowsInvalid;
  pForth->m_pDataStack->vPushI8(iRowsInvalid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid! -- store the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidStore(Forth* pForth)
{
  ENTRY("vJobRowsInvalidStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsInvalid;
  if ( ! pForth->m_pDataStack->bPop(iRowsInvalid) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_ixRowsInvalid = iRowsInvalid;
  
  pJob->m_xResponse.m_ixRowsInvalid = iRowsInvalid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed@ -- fetch the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedFetch(Forth* pForth)
{
  ENTRY("vJobRowsProcessedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsProcessed = *pJob->m_xResponse.m_ixRowsProcessed;
  pForth->m_pDataStack->vPushI8(iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed! -- store the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedStore(Forth* pForth)
{
  ENTRY("vJobRowsProcessedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsProcessed;
  if ( ! pForth->m_pDataStack->bPop(iRowsProcessed) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_ixRowsProcessed = iRowsProcessed;
  
  pJob->m_xResponse.m_ixRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.schemaName@ -- fetch the schema name of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobSchemaNameFetch(Forth* pForth)
{
  ENTRY("vJobSchemaNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sSchemaName;
  
  sSchemaName = pJob->szSchemaName();
  
  pForth->m_pDataStack->vPushS32(sSchemaName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped@ -- fetch the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedFetch(Forth* pForth)
{
  ENTRY("vJobRowsSkippedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
 
  i8 iRowsSkipped = *pJob->m_xResponse.m_ixRowsSkipped;

  pForth->m_pDataStack->vPushI8(iRowsSkipped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped! -- store the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedStore(Forth* pForth)
{
  ENTRY("vJobRowsSkippedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsSkipped;
  if ( ! pForth->m_pDataStack->bPop(iRowsSkipped) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_ixRowsSkipped = iRowsSkipped;
  
  pJob->m_xResponse.m_ixRowsSkipped = iRowsSkipped;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seed@ -- fetch job seed                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedFetch(Forth* pForth)
{
  ENTRY("vJobSeedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_eSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.send -- pre-send of job                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSend(Forth* pForth)
{
  ENTRY("vJobSend"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  // call the job pre-send function
  pJobClass->vSendJob(pJob);  // come back to this, remove session as parameter to delete
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.sessionState@ -- fetch job session state                               */
/*----------------------------------------------------------------------------*/
void Step::vJobSessionStateFetch(Forth* pForth)
{
  ENTRY("vJobSessionStateFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_xReqTuple.m_i_State);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.show -- show the job                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobShow(Forth* pForth)
{
  ENTRY("vJobShow"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  // call the job delete function
  pJobClass->vShowJob(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.signalWaiter -- signal the connector thread                            */
/*----------------------------------------------------------------------------*/
void Step::vJobSignalWaiter(Forth* pForth)
{
  ENTRY("vJobSignalWaiter"_Stz);
  
  // there will be a thread waiting on completion of the job
  // this could be a prompt htread or a java thread via the connector
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pJob->m_bNoWait) {
    //! this is incomplete ... memory leak; job & task need to be deleted
    EXIT();
    return;
  }
  
  if (pJob->m_xReqTuple.m_iDetail)
    JobLog::vNote(pJob,"Connector/Prompt signalled by %s"_Stz,
                       aCN[pJob->eClass()]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT( _bIsValidJob(pJob->eClass()) );
  
  pJob->m_eJobState = JS_SIGNALLED;

  i8 i_State = pJob->m_xReqTuple.m_i_State;
  
  SessionState* pState = AM::pApiState()->pGetState(i_State);
  ApiState::vMarkSignalled(pState->m_pSession);
  RASSERT( _bIsValidJob(pJob->eClass()) );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pForth->m_pTask->vTransition(TR_SIGNAL);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
//void Step::vJobSignalWaiter(Forth* pForth)
//{
//  ENTRY("vJobSignalWaiter"_Stz);
//
//  // there will be a thread waiting on completion of the job
//  // this could be a prompt htread or a java thread via the connector
//
//  Object* pJobPtr;
//  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
//  THROW_DSTACK();
//  Job* pJob = _pJob(pJobPtr);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  if (pJob->m_bNoWait) {
//    EXIT();
//    return;
//  }
//
//  if (pJob->m_xReqTuple.m_iDetail)
//  JobLog::vNote(pJob,"Connector/Prompt signalled by %s"_Stz,
//                aCN[pJob->eClass()]);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  pJob->vLockJobMutex();
//
//  pJob->m_eJobState = JS_SIGNALLED;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  RASSERT( _bIsValidJob(pJob->eClass()) );
//  i8 i_State = pJob->m_xReqTuple.m_i_State;
//
//  SessionState* pState = AM::pApiState()->pGetState(i_State);
//  ApiState::vMarkSignalled(pState->m_pSession);
//  RASSERT( _bIsValidJob(pJob->eClass()) );
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  pJob->vSignalJobMutex();
//
//  pJob->vUnlockJobMutex();
//
//  EXIT();
//}

/*----------------------------------------------------------------------------*/
/* job.slot@ -- fetch job slot                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSlotFetch(Forth* pForth)
{
  ENTRY("vJobSessionSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushI8(pJob->m_i_Slot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn -- spawn the job on the stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawn(Forth* pForth)
{
  ENTRY("vJobSpawn"_Stz);
  
  i8 i_Seed;
  if ( ! pForth->m_pDataStack->bPop(i_Seed) )
    THROW_DSTACK();

  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  AM::vLogSvc(SL_INF,pForth->m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[pForth->m_pTask->m_pJob->eClass()],
              C_ENDARGS);

  pJob->vSeed((eSS)i_Seed);
  IGNORE_RETURN(Api::bSpawnJob(nullptr,pJob));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn_dedicated -- spawn the dedicated job on the stack                */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawnDedicated(Forth* pForth)
{
  ENTRY("vJobSpawnDedicated"_Stz);
  
  i8 i_Seed;
  if ( ! pForth->m_pDataStack->bPop(i_Seed) )
    THROW_DSTACK();
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  AM::vLogSvc(SL_INF,pForth->m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[pForth->m_pTask->m_pJob->eClass()],
              C_ENDARGS);

  pJob->vSeed((eSS)i_Seed);
  IGNORE_RETURN(Api::bSpawnJobDedicated(pJob));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.stateDiagram -- generate state diagram for given job class             */
/*----------------------------------------------------------------------------*/
void Step::vJobStateDiagram(Forth* pForth)
{
  ENTRY("vJobStateDiagram"_Stz);
  
  i8 iClassNo;
  if ( ! pForth->m_pDataStack->bPop(iClassNo) )
    THROW_DSTACK();

  s256 sDiagram;
  
  AM::vStateDiagram(sDiagram,(eCN)iClassNo);
  
  pForth->m_pDataStack->vPushS256(sDiagram);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.Task@ -- fetch the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskFetch(Forth* pForth)
{
  ENTRY("vJobTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  pForth->m_pDataStack->vPushPointer(pJob->pTask());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.Task! -- store the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskStore(Forth* pForth)
{
  ENTRY("vJobTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  Object* pTaskPtr;
  if ( ! pForth->m_pDataStack->bPop(pTaskPtr) )
    THROW_DSTACK();
  Task* pTask = _pTask(pTaskPtr);
  
  // for the moment this should always be null
  ASSERT(! pTask);
  pJob->vTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.tenantName@ -- fetch the tenant name of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobTenantNameFetch(Forth* pForth)
{
  ENTRY("vJobTenantNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s32 sTenantName;
  
  sTenantName = pJob->szTenantName();
  
  pForth->m_pDataStack->vPushS32(sTenantName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::this -- get this job from this task                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobThis(Forth* pForth)
{
  ENTRY("vJobThis"_Stz);
  
  Job* pJob = pForth->m_pTask->m_pJob;
  
  pForth->m_pDataStack->vPushPointer(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::waitParticipant -- wait for participant till found in participant jobs*/
/*----------------------------------------------------------------------------*/
void Step::vJobWaitParticipant(Forth* pForth)
{
  ENTRY("vJobWaitParticipant"_Stz);
  
  i8 iJobId;
  if ( ! pForth->m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  Object* pJobObject;
  if ( ! pForth->m_pDataStack->bPop(pJobObject) )
    THROW_DSTACK();
  
  //Job* pCallingJob = _pJob(pJobObject);
  
  Job__Find* pFindJob = new Job__Find;
  pFindJob->m_iUniqueId = iJobId;

  Job* pJobFound = nullptr;

  while ( ! pJobFound ) {
    AM::pParticipatingJobs()->vGrabLock();
    pJobFound = AM::pParticipatingJobs()->pFindJob(pFindJob);
    AM::pParticipatingJobs()->vDropLock();
  }

  delete pFindJob;
  
//  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(iJobId);
//  RASSERT(pJobFound);
  
  pForth->m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
