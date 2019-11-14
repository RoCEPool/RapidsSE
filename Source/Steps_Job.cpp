/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Job(void)
{
  vAddStep("job::assertCorrectness"_Stz, &Step::vJobAssertCorrectness);
  vAddStep("job::completed"_Stz,         &Step::vJobCompleted);
  vAddStep("job::spawnUniversal"_Stz,    &Step::vJobSpawnUniversal);
  vAddStep("job::this"_Stz,              &Step::vJobThis);

  vAddStep("job.acksRemaining!"_Stz,   &Step::vJobAcksRemaining);
  vAddStep("job.addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("job.addParticipant"_Stz,   &Step::vJobAddParticipant);
  vAddStep("job.assertBegin"_Stz,      &Step::vJobAssertBegin);
  vAddStep("job.assertNeutral"_Stz,    &Step::vJobAssertNeutral);
  vAddStep("job.assertOriginator"_Stz, &Step::vJobAssertOriginator);
  vAddStep("job.assertParticipant"_Stz,&Step::vJobAssertParticipant);
  vAddStep("job.assertReceived"_Stz,   &Step::vJobAssertReceived);

  vAddStep("job.class"_Stz,            &Step::vJobClass);
  vAddStep("job.createResponseVec"_Stz,&Step::vJobCreateResponseVec);
  
  vAddStep("job.dec2pcAcksRmg"_Stz,    &Step::vJobDec2pcAcksRmg);
  vAddStep("job.delete"_Stz,           &Step::vJobDelete);
  
  vAddStep("job.enameSet@"_Stz,        &Step::vJobEnameSetFetch);
  vAddStep("job.error!"_Stz,           &Step::vJobErrorStore);
  vAddStep("job.error@"_Stz,           &Step::vJobErrorFetch);
  vAddStep("job.execute@"_Stz,         &Step::vJobExecuteFetch);

  vAddStep("job::findOriginator"_Stz,  &Step::vJobFindOriginator);
  vAddStep("job::findParticipant"_Stz, &Step::vJobFindParticipant);

  vAddStep("job.hasError"_Stz,         &Step::vJobHasError);
  vAddStep("job.hasOperand"_Stz,       &Step::vJobHasOperand);

  vAddStep("job.id@"_Stz,              &Step::vJobIdFetch);
  vAddStep("job.IsExecuting"_Stz,      &Step::vJobIsExecuting);
  
  vAddStep("job.jobClass"_Stz,         &Step::vJobJobClass);
  vAddStep("job.jobList@"_Stz,         &Step::vJobJobListFetch);
  
  vAddStep("job.metadata!"_Stz,        &Step::vJobMetadataStore);
  
  vAddStep("job.noWait@"_Stz,          &Step::vJobNoWaitFetch);
  
  vAddStep("job.operand@"_Stz,         &Step::vJobOperandFetch);
  vAddStep("job.operand!"_Stz,         &Step::vJobOperandStore);
  
  vAddStep("job.opLoop"_Stz,           &Step::vJobOpLoop);
  vAddStep("job.opTable!"_Stz,         &Step::vJobOpTableStore);
  
  vAddStep("job.originatingId@"_Stz,   &Step::vJobOriginatingIdFetch);
  vAddStep("job.originatingId!"_Stz,   &Step::vJobOriginatingIdStore);
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
  vAddStep("job.rowsInvalid@"_Stz,     &Step::vJobRowsInvalidFetch);
  vAddStep("job.rowsInvalid!"_Stz,     &Step::vJobRowsInvalidStore);
  vAddStep("job.rowsProcessed@"_Stz,   &Step::vJobRowsProcessedFetch);
  vAddStep("job.rowsProcessed!"_Stz,   &Step::vJobRowsProcessedStore);
  vAddStep("job.rowsSkipped@"_Stz,     &Step::vJobRowsSkippedFetch);
  vAddStep("job.rowsSkipped!"_Stz,     &Step::vJobRowsSkippedStore);

  vAddStep("job.seed@"_Stz,            &Step::vJobSeedFetch);
  vAddStep("job.send"_Stz,             &Step::vJobSend);
  vAddStep("job.session@"_Stz,         &Step::vJobSessionFetch);
  vAddStep("job.sessionState@"_Stz,    &Step::vJobSessionStateFetch);
  vAddStep("job.show"_Stz,             &Step::vJobShow);
  vAddStep("job.signalWaiter"_Stz,     &Step::vJobSignalWaiter);
  vAddStep("job.slot@"_Stz,            &Step::vJobSlotFetch);
  vAddStep("job.spawn"_Stz,            &Step::vJobSpawn);
  vAddStep("job.spawn_dedicated"_Stz,  &Step::vJobSpawnDedicated);
  vAddStep("job::stateDiagram"_Stz,    &Step::vJobStateDiagram);

  vAddStep("job.task@"_Stz,            &Step::vJobTaskFetch);
  vAddStep("job.task!"_Stz,            &Step::vJobTaskStore);
 
  vAddStep("job::waitParticipant"_Stz, &Step::vJobWaitParticipant);
}


/*----------------------------------------------------------------------------*/
/* job.acksRemaining! -- set acks remaining for this job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobAcksRemaining(void)
{
  ENTRY("vJobAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 nAcksRemaining;
  if ( ! m_pDataStack->bPopI8(nAcksRemaining) )
    THROW_DSTACK();
  
  pJob->vAcksRmg(nAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addOriginator -- add job to originator job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobAddOriginator(void)
{
  ENTRY("vJobAddOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
    AM::pOriginatingJobs()->vLogJobs(pJob->m_iFocalId);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addParticipant -- add job to participant job set                       */
/*----------------------------------------------------------------------------*/
void Step::vJobAddParticipant(void)
{
  ENTRY("vJobAddParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
    AM::pParticipatingJobs()->vLogJobs(pJob->m_iFocalId);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertBegin -- assert job is not in originator nor participant set     */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertBegin(void)
{
  ENTRY("vJobAssertBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (Job::iNodeNo(pJob->m_iFocalId) == AM::pGrid()->m_iThisNode) {
    
    if (! pJob->m_bWasReceived)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "job.assertOriginator: "
             "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    if (JL_NONE != pJob->m_eJobList)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "job.assertBegin: "
             "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iFocalId);
    if (! pJobFound)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "job.assertBegin: "
             "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iFocalId);
    if (pJobFound)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "job.assertBegin: "
             "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pJob->m_bWasReceived)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  if (JL_NONE != pJob->m_eJobList)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertBegin: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iFocalId);
  if (pJobFound)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertBegin: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iFocalId);
  if (pJobFound)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertBegin: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::assertCorrectness -- assert correctness                               */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertCorrectness(void)
{
  ENTRY("vJobAssertCorrectness"_Stz);
  
  //Job* pJob = m_pTask->m_pJob;
  
  //vTrace("job::assertCorrectness\n"_Stz);
  
  m_pTask->vAssertStructure();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertNeutral -- assert job is not in originator nor participant set   */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertNeutral(void)
{
  ENTRY("vJobAssertNeutral"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (JL_NONE != pJob->m_eJobList)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertNeutral: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iFocalId);
  if (pJobFound)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertNeutral: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iFocalId);
  if (pJobFound)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertNeutral: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertOriginator -- assert job is in originator job set                */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertOriginator(void)
{
  ENTRY("vJobAssertOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (JL_ORIGINATING != pJob->m_eJobList)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_eJobList is not JL_ORIGINATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  if (pJob->m_bWasReceived)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_bWasReceived is true [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->m_iFocalId);
  if (! pJobFound)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertParticipant -- assert job is in participant job set              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertParticipant(void)
{
  ENTRY("vJobAssertParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (JL_PARTICIPATING != pJob->m_eJobList)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertParticipant: "
           "Job::m_eJobList is not JL_PARTICIPATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  if (! pJob->m_bWasReceived)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->m_iFocalId);
  if (! pJobFound)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertParticipant: "
           "Job not found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertReceived -- assert job was received                              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertReceived(void)
{
  ENTRY("vJobAssertReceived"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  if (! pJob->m_bWasReceived)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertReceived: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClass(void)
{
  ENTRY("vJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  eCN eClass = pJob->eClass();
  
  m_pDataStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::completed --                                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobCompleted(void)
{
  ENTRY("vJobCompleted"_Stz);
  
  Job* pJob = m_pTask->pJob();
  
  pJob->m_eJobState = JS_COMPLETED;
  
  m_pTask->vTransition(TR_DELETE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.createResponseVec -- create the response vector in the job             */
/*----------------------------------------------------------------------------*/
void Step::vJobCreateResponseVec(void)
{
  ENTRY("vJobCreateResponseVec"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  RASSERT( ! pJob->m_pResponseVec );
  pJob->m_pResponseVec = new ResponseVec;
  pJob->m_pBufferQueue = new Queue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.operand@ -- fetch operand from job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobOperandFetch(void)
{
  ENTRY("vJobOperandFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushPointer(pJob->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.operand! -- store operand in the job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobOperandStore(void)
{
  ENTRY("vJobOperandStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  Object* pOperandPtr;
  if ( ! m_pDataStack->bPopObject(pOperandPtr) )
    THROW_DSTACK();
  Operand* pOperand = _pOperand(pOperandPtr);
  
  pJob->m_pOperand = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.opLoop -- execute the job's op loop                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobOpLoop(void)
{
  ENTRY("vJobOperandStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Universal* pJob_Universal = _pJob_Universal(pJobPtr);
  
  Op* pOp = pJob_Universal->m_pOp;
  pOp->vOpLoop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.opTable! -- store the table in the job's op                            */
/*----------------------------------------------------------------------------*/
void Step::vJobOpTableStore(void)
{
  ENTRY("vJobOpTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Universal* pJob_Universal = _pJob_Universal(pJobPtr);
  
  Object* pTablePtr;
  if ( ! m_pDataStack->bPopObject(pTablePtr) )
    THROW_DSTACK();
  Table* pTable = _pTable(pTablePtr);
  
  Op* pOp = pJob_Universal->m_pOp;
  pOp->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.delete -- delete the job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobDelete(void)
{
  ENTRY("vJobDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
/* job.dec2pcAcksRmg -- decrement acks remaining for this job                 */
/*----------------------------------------------------------------------------*/
void Step::vJobDec2pcAcksRmg(void)
{
  ENTRY("vJobDec2pcAcksRmg"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  bool bAmDecider = pJob->bAcksRmgDec();
  
  m_pDataStack->vPushI8(bAmDecider);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.enameSet@ -- push ptr to job EnameSet                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobEnameSetFetch(void)
{
  ENTRY("vJobEnameSetFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushPointer(&pJob->m_oEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error@ -- fetch the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorFetch(void)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);

  i8 iNode;
  if ( ! m_pDataStack->bPopAsInt(iNode) )
    THROW_DSTACK();
  
  m_pDataStack->vPushI8(pJob->m_xResponse.m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error! -- store the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorStore(void)
{
  ENTRY("vJobErrorStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobError;
  if ( ! m_pDataStack->bPopAsInt(iJobError) )
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
void Step::vJobExecuteFetch(void)
{
  ENTRY("vJobExecuteFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
  
  m_pDataStack->vPushS32(sExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findOriginator -- find job from originator job set                    */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(void)
{
  ENTRY("vJobFindOriginator"_Stz);
  
  i8 iJobId;
  if ( ! m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  Job* pJobFound = Scheduler::pFindOriginatingJob(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pRegSet->m_xpJobRef);
  
  
  m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findParticipant -- find job from participant job set                  */
/*----------------------------------------------------------------------------*/
void Step::vJobFindParticipant(void)
{
  ENTRY("vJobFindParticipant"_Stz);
  
  i8 iJobId;
  if ( ! m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
    
  Job* pJobFound = Scheduler::pFindParticipatingJob(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pRegSet->m_xpJobRef);
  
  
  m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasError -- return true if job has error                               */
/*----------------------------------------------------------------------------*/
void Step::vJobHasError(void)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_bHasError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasOperand -- return true if job has an operand                        */
/*----------------------------------------------------------------------------*/
void Step::vJobHasOperand(void)
{
  ENTRY("vJobHasOperand"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  
  i8 bHas = pJob->m_pOperand ? true : false;
  if (pJob->m_aJobClass->m_eClass == CN_JOB_WRITEBUFFER)
    int iDebug = 1;
  
  m_pDataStack->vPushI8(bHas);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch(void)
{
  ENTRY("vJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iFocalId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.isExecuting -- is the job executing (SEED_EXE)                         */
/*----------------------------------------------------------------------------*/
void Step::vJobIsExecuting(void)
{
  ENTRY("vJobIsExecuting"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iIsExecuting = pJob->eSeed() == SS_SEED_EXE ? 1 : 0;
  m_pDataStack->vPushI8(iIsExecuting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobClass -- get the JobClass of the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobJobClass(void)
{
  ENTRY("vJobJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobClass_p = (i8)pJob->eClass();
  
  m_pDataStack->vPushI8(iJobClass_p);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobList@ -- fetch job list                                             */
/*----------------------------------------------------------------------------*/
void Step::vJobJobListFetch(void)
{
  ENTRY("vJobJobListFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_eJobList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.metadata! -- store meta data string in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJobMetadataStore(void)
{
  ENTRY("vJobMetadataStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s4K sMetadata;
  if ( ! m_pDataStack->bPopS4K(sMetadata) )
    THROW_DSTACK();
  
  pJob->m_sMetadata = sMetadata;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.noWait@ -- fetch no-wait                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobNoWaitFetch(void)
{
  ENTRY("vJobNoWaitFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_bNoWait);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingId@ -- fetch originating id from job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdFetch(void)
{
  ENTRY("vJobOriginatingIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iOriginatingId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingId! -- store originating id into job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdStore(void)
{
  ENTRY("vJobOriginatingIdStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iOriginatingId;
  if ( ! m_pDataStack->bPopI8(iOriginatingId) )
    THROW_DSTACK();
  
  pJob->m_iOriginatingId = iOriginatingId;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask@ -- fetch the originating task from the job            */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskFetch(void)
{
  ENTRY("vJobOriginatingTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushPointer(pJob->m_pOriginatingTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask! -- store the originating task in the job              */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskStore(void)
{
  ENTRY("vJobOriginatingTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  Object* pOriginatingTaskPtr;
  if ( ! m_pDataStack->bPopObject(pOriginatingTaskPtr) )
    THROW_DSTACK();
  Task* pOriginatingTask = _pTask(pOriginatingTaskPtr);
  ASSERT(CN_TASK == pOriginatingTask->eClass());
  
  pJob->m_pOriginatingTask = pOriginatingTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::originator@ -- fetch the originator node from the job                  */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatorFetch(void)
{
  ENTRY("vJobOriginatorFetch"_Stz);
  
  i8 iJobId;
  if ( ! m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  m_pDataStack->vPushI8(Job::iOriginatorNode(iJobId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.parentId@ -- fetch job parent id                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobParentIdFetch(void)
{
  ENTRY("vJobParentIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iParentId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participant@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantFetch(void)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iParticipantNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partcipant! -- store the participant node in the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantStore(void)
{
  ENTRY("vJobParticipantStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iParticipantNode;
  if ( ! m_pDataStack->bPopAsInt(iParticipantNode) )
    THROW_DSTACK();
  
  pJob->m_iParticipantNode = iParticipantNode;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantHasError -- has any participant returned an error ?         */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantHasError(void)
{
  ENTRY("job.participantHasError"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
  m_pDataStack->vPushI8(bHasError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantResponse! -- store a job response for the participant       */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantResponseStore(void)
{
  ENTRY("vJobParticipantResponseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
//  i8 iNode;
//  if ( ! m_pDataStack->bPopAsInt(iNode) )
//    THROW_DSTACK();
  
  Object* pParticipantJobPtr;
  if ( ! m_pDataStack->bPopObject(pParticipantJobPtr) )
    THROW_DSTACK();
  Job* pParticipantJob = _pJob(pParticipantJobPtr);
  
  // there should be a response vector
  RASSERT( pJob->m_pResponseVec );
  
  // create a new response
  Response* pResponse = new Response;
  
  // set node and error in the response
  //RASSERT(iNode == pParticipantJob->m_iParticipantNode);
  pResponse->m_iNode           = pParticipantJob->m_iParticipantNode;
  pResponse->m_sResponse       = pParticipantJob->m_xResponse.m_sResponse;
  pResponse->m_ixRowsProcessed = pParticipantJob->m_xResponse.m_ixRowsProcessed;
  pResponse->m_ixRowsInvalid   = pParticipantJob->m_xResponse.m_ixRowsInvalid;
  pResponse->m_ixRowsSkipped   = pParticipantJob->m_xResponse.m_ixRowsSkipped;
  pResponse->m_eErrorCode      = pParticipantJob->m_xResponse.m_eErrorCode;
  
  if (pResponse->m_eErrorCode != EC_NONE)
    pJob->m_bHasError = true;
  
  // add the response to the vector of responses
  pJob->m_pResponseVec->vAdd(pResponse);
  
  //if (pResponse->m_eErrorCode != EC_NONE)
    //ActLog::vAdd(pJob,AN_ERROR_CODE,pResponse->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partitions@ -- fetch partitions from job_sanity                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPartitionsFetch(void)
{
  ENTRY("vJobPartitionsFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  m_pDataStack->vPushI8(pJob_Sanity->m_iPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase@ -- fetch the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(void)
{
  ENTRY("vJobPhaseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_eJobPhase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase! -- store the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseStore(void)
{
  ENTRY("vJobPhaseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobPhase;
  if ( ! m_pDataStack->bPopAsInt(iJobPhase) )
    THROW_DSTACK();
  
  pJob->m_eJobPhase = (eJP)iJobPhase;
  
  //ActLog::vAdd(pJob,AN_JOB_PHASE,iJobPhase,_EG_JP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.postRecv -- run the post-receive function of the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobPostRecv(void)
{
  ENTRY("vJobPostRecv"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the job class on this node
  
  JobClass* aJobClass = nullptr;
  eCN eClass = pJob->eClass();
  
  if (CN_JOB_UNIVERSAL == eClass) {
    eClass = (eCN)pJob->m_oEnameSet.iValue("<jobClass>"_Stz);
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
  
  //ActLog::vAdd(pJob,AN_POST_RECV,m_pTask->pJob());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.preSend -- run the pre-send function of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPreSend(void)
{
  ENTRY("vJobPreSend"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  JobClass* pJobClass = pJob->m_aJobClass;
  
  //ActLog::vAdd(pJob,AN_PRE_SEND,m_pTask->pJob());
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
void Step::vJobRecv(void)
{
  ENTRY("vJobRecv"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
void Step::vJobRemoveOriginator(void)
{
  ENTRY("vJobRemoveOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  bool bRemove = Scheduler::bRemoveOriginatingJob(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from originator\n"_Stz,
           aCN[pJob->eClass()],pJob->m_iFocalId);
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = Scheduler::bRemoveOriginatingJob(pJob,true);
  }
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,
                       "Removed from Originator List: %s %p"_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    AM::pOriginatingJobs()->vLogJobs(pJob->m_iFocalId);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeParticipant -- remove job from participant job set               */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveParticipant(void)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  bool bRemove = Scheduler::bRemoveParticipatingJob(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from participant\n"_Stz,
           aCN[pJob->eClass()],pJob->m_iFocalId);
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = Scheduler::bRemoveParticipatingJob(pJob,true);
  }
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Removed from Participant List: "_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    AM::pParticipatingJobs()->vLogJobs(pJob->m_iFocalId);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.regionSizeResponse@ -- fetch region size from job_sanity               */
/*----------------------------------------------------------------------------*/
void Step::vJobRegionSizeFetch(void)
{
  ENTRY("vJobRegionSizeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  m_pDataStack->vPushI8(pJob_Sanity->m_iRegionSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.reseed -- reseed the job on the stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobReseed(void)
{
  ENTRY("vJobReseed"_Stz);
  
  i8 i_Seed;
  if ( ! m_pDataStack->bPopI8(i_Seed) )
    THROW_DSTACK();
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_ENDARGS);
  
  pJob->vSeed((eSS)i_Seed);
  IGNORE_RETURN(Api::bReseedJob(nullptr,pJob));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response@ -- fetch the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseFetch(void)
{
  ENTRY("vJobResponseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushS4K(pJob->m_xResponse.m_sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response! -- store the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseStore(void)
{
  ENTRY("vJobResponseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  s4K sResponse;
  if ( ! m_pDataStack->bPopS4K(sResponse) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_sResponse = sResponse;
  pJob->m_xResponse.m_sResponse = sResponse;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid@ -- fetch the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidFetch(void)
{
  ENTRY("vJobRowsInvalidFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsInvalid = *pJob->m_xResponse.m_ixRowsInvalid;
  m_pDataStack->vPushI8(iRowsInvalid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid! -- store the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidStore(void)
{
  ENTRY("vJobRowsInvalidStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsInvalid;
  if ( ! m_pDataStack->bPopI8(iRowsInvalid) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_ixRowsInvalid = iRowsInvalid;
  
  pJob->m_xResponse.m_ixRowsInvalid = iRowsInvalid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed@ -- fetch the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedFetch(void)
{
  ENTRY("vJobRowsProcessedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsProcessed = *pJob->m_xResponse.m_ixRowsProcessed;
  m_pDataStack->vPushI8(iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed! -- store the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedStore(void)
{
  ENTRY("vJobRowsProcessedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsProcessed;
  if ( ! m_pDataStack->bPopI8(iRowsProcessed) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_ixRowsProcessed = iRowsProcessed;
  
  pJob->m_xResponse.m_ixRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped@ -- fetch the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedFetch(void)
{
  ENTRY("vJobRowsSkippedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
 
  i8 iRowsSkipped = *pJob->m_xResponse.m_ixRowsSkipped;

  m_pDataStack->vPushI8(iRowsSkipped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped! -- store the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedStore(void)
{
  ENTRY("vJobRowsSkippedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iRowsSkipped;
  if ( ! m_pDataStack->bPopI8(iRowsSkipped) )
    THROW_DSTACK();
  
  //pJob->m_xNode[iNode].m_ixRowsSkipped = iRowsSkipped;
  
  pJob->m_xResponse.m_ixRowsSkipped = iRowsSkipped;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seed@ -- fetch job seed                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedFetch(void)
{
  ENTRY("vJobSeedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->eSeed());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.send -- pre-send of job                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSend(void)
{
  ENTRY("vJobSend"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
/* job.session@ -- fetch job session                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobSessionFetch(void)
{
  ENTRY("vJobSessionStateFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  i8 iSessionId = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = nullptr;
  
  if (iSessionId != C_UNKNOWN)
    pSession = AM::pSessionArray()->pGetSession(iSessionId);
  
  m_pDataStack->vPushPointer(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.sessionState@ -- fetch job session state                               */
/*----------------------------------------------------------------------------*/
void Step::vJobSessionStateFetch(void)
{
  ENTRY("vJobSessionStateFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_xReqTuple.m_i_Session);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.show -- show the job                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobShow(void)
{
  ENTRY("vJobShow"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
void Step::vJobSignalWaiter(void)
{
  ENTRY("vJobSignalWaiter"_Stz);
  
  // there will be a thread waiting on completion of the job
  // this could be a prompt htread or a java thread via the connector
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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

  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);
  SessionArray::vMarkSignalled(pSession);
  RASSERT( _bIsValidJob(pJob->eClass()) );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pTask->vTransition(TR_SIGNAL);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
//void Step::vJobSignalWaiter(void)
//{
//  ENTRY("vJobSignalWaiter"_Stz);
//
//  // there will be a thread waiting on completion of the job
//  // this could be a prompt htread or a java thread via the connector
//
//  Object* pJobPtr;
//  if ( ! m_pDataStack->bPopObject(pJobPtr) )
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
void Step::vJobSlotFetch(void)
{
  ENTRY("vJobSessionSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->i_Slot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn -- spawn the job on the stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawn(void)
{
  ENTRY("vJobSpawn"_Stz);
  
  i8 i_Seed;
  if ( ! m_pDataStack->bPopI8(i_Seed) )
    THROW_DSTACK();

  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_ENDARGS);

  pJob->vSeed((eSS)i_Seed);
  IGNORE_RETURN(Api::bSpawnJob(pJob));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn_dedicated -- spawn the dedicated job on the stack                */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawnDedicated(void)
{
  ENTRY("vJobSpawnDedicated"_Stz);
  
  i8 i_Seed;
  if ( ! m_pDataStack->bPopI8(i_Seed) )
    THROW_DSTACK();
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_ENDARGS);

  pJob->vSeed((eSS)i_Seed);
  IGNORE_RETURN(Api::bSpawnJobDedicated(pJob));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawnUniversal -- spawn a universal job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawnUniversal(void)
{
  ENTRY("vJobSpawnUniversal"_Stz);
  
  s32 sJobNm;
  if ( ! m_pDataStack->bPopS32(sJobNm) )
    THROW_DSTACK();

  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob_parent = _pJob(pJobPtr);
  
  EnameSet* pEnameSet_jobInfo = Job::pJobInfo_enameSet(sJobNm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // find the job class
  JobClass* aJobClass = JobClass::pFind(CN_JOB_UNIVERSAL);

  // new the universal job
  ReqTuple xReqTuple("!!vJobSpawnUniversal"_Stz,false);
  Job_Universal* pJob_Universal = new Job_Universal(aJobClass,&xReqTuple);

  // ranslate job type and kind
  pJob_Universal->m_eJobType = (eJT) pEnameSet_jobInfo->iValue("<?jobType?>"_Stz);
  pJob_Universal->m_eJobKind = (eJK) pEnameSet_jobInfo->iValue("<?jobKind?>"_Stz);

  // add the necessary ename tags
  EnameSet* pEnameSet_job = & pJob_Universal->m_oEnameSet;
  pEnameSet_job->vAdd("<jobClass>"_Stz,CN_JOB_UNIVERSAL);
  pEnameSet_job->vAdd("<jobNm>"_Stz,sJobNm);
  pEnameSet_job->vAdd("<path>"_Stz,"/"_Stz);

  // seed the job for request
  pJob_Universal->vSeed(SS_SEED_REQ);

  // spawn the job
  IGNORE_RETURN(Api::bSpawnJob(pJob_Universal));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.stateDiagram -- generate state diagram for given job class             */
/*----------------------------------------------------------------------------*/
void Step::vJobStateDiagram(void)
{
  ENTRY("vJobStateDiagram"_Stz);
  
  i8 iClassNo;
  if ( ! m_pDataStack->bPopI8(iClassNo) )
    THROW_DSTACK();

  s256 sDiagram;
  
  AM::vStateDiagram(sDiagram,(eCN)iClassNo);
  
  m_pDataStack->vPushS256(sDiagram);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.Task@ -- fetch the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskFetch(void)
{
  ENTRY("vJobTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  m_pDataStack->vPushPointer(pJob->pTask());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.Task! -- store the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskStore(void)
{
  ENTRY("vJobTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);
  
  Object* pTaskPtr;
  if ( ! m_pDataStack->bPopObject(pTaskPtr) )
    THROW_DSTACK();
  Task* pTask = _pTask(pTaskPtr);
  
  // for the moment this should always be null
  ASSERT(! pTask);
  pJob->vTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::this -- get this job from this task                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobThis(void)
{
  ENTRY("vJobThis"_Stz);
  
  Job* pJob = m_pTask->m_pJob;
  
  m_pDataStack->vPushPointer(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::waitParticipant -- wait for participant till found in participant jobs*/
/*----------------------------------------------------------------------------*/
void Step::vJobWaitParticipant(void)
{
  ENTRY("vJobWaitParticipant"_Stz);
  
  i8 iJobId;
  if ( ! m_pDataStack->bPopAsInt(iJobId) )
    THROW_DSTACK();
  
  Object* pJobObject;
  if ( ! m_pDataStack->bPopObject(pJobObject) )
    THROW_DSTACK();
  
  //Job* pCallingJob = _pJob(pJobObject);
  
  Job__Find* pFindJob = new Job__Find;
  pFindJob->m_iFocalId = iJobId;

  Job* pJobFound = nullptr;

  while ( ! pJobFound ) {
    AM::pParticipatingJobs()->vGrabLock();
    pJobFound = AM::pParticipatingJobs()->pFindJob(pFindJob);
    AM::pParticipatingJobs()->vDropLock();
  }

  delete pFindJob;
  
//  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(iJobId);
//  RASSERT(pJobFound);
  
  m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
