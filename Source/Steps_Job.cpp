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

  vAddStep("job.bideOperand"_Stz,      &Step::vJobBideOperand);

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
  
  vAddStep("job.kind@"_Stz,            &Step::vJobKindFetch);
  
  vAddStep("job.LoopOutputOp"_Stz,     &Step::vJobLoopOutputOp);
  
  vAddStep("job.metadata!"_Stz,        &Step::vJobMetadataStore);
  
  vAddStep("job.nextOperand"_Stz,      &Step::vJobNextOperand);
  vAddStep("job.noWait@"_Stz,          &Step::vJobNoWaitFetch);
  
  vAddStep("job.operand@"_Stz,         &Step::vJobOperandFetch);
  vAddStep("job.operand!"_Stz,         &Step::vJobOperandStore);
  
  vAddStep("job.openToFile"_Stz,       &Step::vJobOpenToFile);
  
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

  vAddStep("job.seedDecode"_Stz,       &Step::vJobSeedDecode);
  vAddStep("job.seed@"_Stz,            &Step::vJobSeedFetch);
  vAddStep("job.seed!"_Stz,            &Step::vJobSeedStore);
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
  ENTRY(); // ENTRY("vJobAcksRemaining"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 nAcksRemaining = m_pDataStack->iPopInt(__func__);
  
  pJob->vAcksRmg(nAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addOriginator -- add job to originator job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobAddOriginator(void)
{
  ENTRY(); // ENTRY("vJobAddOriginator"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
    
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
  ENTRY(); // ENTRY("vJobAddParticipant"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobAssertBegin"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  if (Job::iNodeNo(pJob->m_iFocalId) == AM::pGrid()->m_iThisNode) {
    
    if (! pJob->bWasReceived())
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "job.assertOriginator: "
             "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
    
    if (JL_NONE != pJob->eJobList())
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
  
  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  if (JL_NONE != pJob->eJobList())
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
  ENTRY(); // ENTRY("vJobAssertCorrectness"_Stz);
  
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
  ENTRY(); // ENTRY("vJobAssertNeutral"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  if (JL_NONE != pJob->eJobList())
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
  ENTRY(); // ENTRY("vJobAssertOriginator"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  if (JL_ORIGINATING != pJob->eJobList())
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_eJobList is not JL_ORIGINATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);

  if (pJob->bWasReceived())
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
  ENTRY(); // ENTRY("vJobAssertParticipant"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  if (JL_PARTICIPATING != pJob->eJobList())
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertParticipant: "
           "Job::m_eJobList is not JL_PARTICIPATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  if (! pJob->bWasReceived())
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
  ENTRY(); // ENTRY("vJobAssertReceived"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "job.assertReceived: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eClass()],AM::pGrid()->m_iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.bideOperand -- get next operand                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobBideOperand(void)
{
  ENTRY(); // ENTRY("vJobBideOperand"_Stz);
  
  Job_Universal* pJob_Universal = _pJob_Universal(m_pDataStack->pPopPtr(__func__));
  
  m_pRegSet->m_pOperand = pJob_Universal->m_pOp->pPopQueue2();
  
  if (! m_pRegSet->m_pOperand) {
    ASSERT(m_pTask->bIsLocked());
    m_pTask->vSchedulerAct(SA_BIDE);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClass(void)
{
  ENTRY(); // ENTRY("vJobClass"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  eCN eClass = pJob->eClass();
  
  m_pDataStack->vPushInt(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::completed --                                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobCompleted(void)
{
  ENTRY(); // ENTRY("vJobCompleted"_Stz);
  
  Job* pJob = m_pTask->pJob();
  
  pJob->vJobState(JS_COMPLETED);
  
  m_pTask->vSchedulerAct(SA_DELETE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.createResponseVec -- create the response vector in the job             */
/*----------------------------------------------------------------------------*/
void Step::vJobCreateResponseVec(void)
{
  ENTRY(); // ENTRY("vJobCreateResponseVec"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  RASSERT( ! pJob->m_pResponseVec );
  pJob->m_pResponseVec = new ResponseVec;
  pJob->m_pBufferQueue = new Queue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.delete -- delete the job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobDelete(void)
{
  ENTRY(); // ENTRY("vJobDelete"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobDec2pcAcksRmg"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  bool bAmDecider = pJob->bAcksRmgDec();
  
  m_pDataStack->vPushInt(bAmDecider);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.enameSet@ -- push ptr to job EnameSet                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobEnameSetFetch(void)
{
  ENTRY(); // ENTRY("vJobEnameSetFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->pEnameSet());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error@ -- fetch the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorFetch(void)
{
  ENTRY(); // ENTRY("vJobErrorFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  i8 iNode = m_pDataStack->iPopAsInt(__func__);
  
  m_pDataStack->vPushInt(pJob->m_xResponse.m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error! -- store the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorStore(void)
{
  ENTRY(); // ENTRY("vJobErrorStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobError = m_pDataStack->iPopAsInt(__func__);
  
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
  ENTRY(); // ENTRY("vJobExecuteFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s32 sExecute;

  switch (pJob->eClass()) {
            
    default:
      StrLib::vCopy(sExecute.azChar(),pJob->m_aJobClass->m_aSeedExe->azChar());
      //sExecute.vConcat("_exe"_Stz);
      break;
  }
  
  m_pDataStack->vPushStr(sExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findOriginator -- find job from originator job set                    */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(void)
{
  ENTRY(); // ENTRY("vJobFindOriginator"_Stz);
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  Job* pJobFound = Scheduler::pFindOriginatingJob(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pRegSet->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findParticipant -- find job from participant job set                  */
/*----------------------------------------------------------------------------*/
void Step::vJobFindParticipant(void)
{
  ENTRY(); // ENTRY("vJobFindParticipant"_Stz);
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
    
  Job* pJobFound = Scheduler::pFindParticipatingJob(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pRegSet->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasError -- return true if job has error                               */
/*----------------------------------------------------------------------------*/
void Step::vJobHasError(void)
{
  ENTRY(); // ENTRY("vJobErrorFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_bHasError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasOperand -- return true if job has an operand                        */
/*----------------------------------------------------------------------------*/
void Step::vJobHasOperand(void)
{
  ENTRY(); // ENTRY("vJobHasOperand"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  
  i8 bHas = pJob->m_pOperand ? true : false;
  if (pJob->m_aJobClass->m_eClass == CN_JOB_WRITEBUFFER)
    int iDebug = 1;
  
  m_pDataStack->vPushInt(bHas);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch(void)
{
  ENTRY(); // ENTRY("vJobIdFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_iFocalId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.isExecuting -- is the job executing (SEED_EXE)                         */
/*----------------------------------------------------------------------------*/
void Step::vJobIsExecuting(void)
{
  ENTRY(); // ENTRY("vJobIsExecuting"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iIsExecuting = pJob->eSeed() == SS_SEED_EXE ? 1 : 0;
  m_pDataStack->vPushInt(iIsExecuting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobClass -- get the JobClass of the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobJobClass(void)
{
  ENTRY(); // ENTRY("vJobJobClass"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobClass_p = (i8)pJob->eClass();
  
  m_pDataStack->vPushInt(iJobClass_p);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobList@ -- fetch job list                                             */
/*----------------------------------------------------------------------------*/
void Step::vJobJobListFetch(void)
{
  ENTRY(); // ENTRY("vJobJobListFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eJobList());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.kind@ -- fetch job kind                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobKindFetch(void)
{
  ENTRY(); // ENTRY("vJobKindFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  
  m_pDataStack->vPushInt(pJob->eJobKind());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.LoopOutputOp -- execute the job's output op loop                       */
/*----------------------------------------------------------------------------*/
void Step::vJobLoopOutputOp(void)
{
  ENTRY(); // ENTRY("vJobLoopOutputOp"_Stz);
  
  Job_Universal* pJob_Universal = _pJob_Universal(m_pDataStack->pPopPtr(__func__));
  
  bool bContinue = true;
  
  Op* pOp = pJob_Universal->m_pOp;
  
  for (;;) {
    
    Operand* pOperand = pOp->pPopQueue2();
    
    if (! pOperand) {
      ASSERT(pOp->bIsLocked());
      AM::vTaskSchedulerAct(m_pTask,SA_BIDE);
      goto L_EXIT;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
        // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        pOp->m_iWorkDone += pCrwBuffer->nRowCount();
        pOp->vPipeToJob(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
        pOp->m_iWorkDone += pCsvBuffer->nRecords();
        pOp->vPipeToJob(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_ENDMARKER: {
        
        if (--pOp->m_nLeftEndMarkers == 0) {
          if (pOp->bIsCancelling()) {
            delete _pEndMarker(pOperand);
            pOp->vPipeToJob(new CancelMarker);
          }
          else
            pOp->vPipeToJob(pOperand);
          
          if (AM::bJobHasError(pOp->m_pJobUniversal)) {
            AM::vSetJobLocalError(pOp->m_pJobUniversal, pOp->m_pPipeline->pJob());
          }
          if (pOp->m_bAckJobEnd)
            pOp->vAckJob();
          
          bContinue = false;
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        pOp->m_iWorkDone += pEntBuffer->nItems();
        pOp->vPipeToJob(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        pOp->m_iWorkDone += pIrwBuffer->nItems();
        pOp->vPipeToJob(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        pOp->m_iWorkDone += pSrtBuffer->nItems();
        pOp->vPipeToJob(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        pOp->vPipeToJob(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      default:
        RASSERT(0);
        break;
    }
  
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  
  m_pDataStack->vPushInt(bContinue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.metadata! -- store meta data string in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJobMetadataStore(void)
{
  ENTRY(); // ENTRY("vJobMetadataStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s4K sMetadata;
  m_pDataStack->vPopStr(sMetadata,__func__);
  
  pJob->m_sMetadata = sMetadata;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.nextOperand -- get next operand                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobNextOperand(void)
{
  ENTRY(); // ENTRY("vJobNextOperand"_Stz);
  
  Job_Universal* pJob_Universal = _pJob_Universal(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* pOp = pJob_Universal->m_pOp;
  
  if (! m_pRegSet->m_pOperand)
    m_pRegSet->m_pOperand = pOp->pPopQueue2();
  ASSERT(m_pRegSet->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.noWait@ -- fetch no-wait                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobNoWaitFetch(void)
{
  ENTRY(); // ENTRY("vJobNoWaitFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->bNoWaitTag());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.openToFile -- open the toFile                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobOpenToFile(void)
{
  ENTRY(); // ENTRY("vJobOpenToFile"_Stz);
  
  Job_Universal* pJob_Universal = _pJob_Universal(m_pDataStack->pPopPtr(__func__));
  
  s4K sFilePath;
  Op* pToFileOp = pJob_Universal->m_pOp;
  
  sFilePath = pToFileOp->m_pOpSpec->pEnameSet()->szValue((i8)0);
  if (! m_pRegSet->m_xpFile0->bOpenWrite(sFilePath)) {
    vTrace("Failed to open file: %s\n"_Stz,sFilePath.azChar());
    RASSERT(0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.operand@ -- fetch operand from job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobOperandFetch(void)
{
  ENTRY(); // ENTRY("vJobOperandFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.operand! -- store operand in the job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobOperandStore(void)
{
  ENTRY(); // ENTRY("vJobOperandStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  pJob->m_pOperand = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.opTable! -- store the table in the job's op                            */
/*----------------------------------------------------------------------------*/
void Step::vJobOpTableStore(void)
{
  ENTRY(); // ENTRY("vJobOpTableStore"_Stz);
  
  Job_Universal* pJob_Universal = _pJob_Universal(m_pDataStack->pPopPtr(__func__));
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  Op* pOp = pJob_Universal->m_pOp;
  pOp->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingId@ -- fetch originating id from job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdFetch(void)
{
  ENTRY(); // ENTRY("vJobOriginatingIdFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_iOriginatingId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingId! -- store originating id into job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdStore(void)
{
  ENTRY(); // ENTRY("vJobOriginatingIdStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iOriginatingId = m_pDataStack->iPopInt(__func__);
  
  pJob->m_iOriginatingId = iOriginatingId;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask@ -- fetch the originating task from the job            */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskFetch(void)
{
  ENTRY(); // ENTRY("vJobOriginatingTaskFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->m_pOriginatingTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask! -- store the originating task in the job              */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskStore(void)
{
  ENTRY(); // ENTRY("vJobOriginatingTaskStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pOriginatingTask = _pTask(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TASK == pOriginatingTask->eClass());
  
  pJob->m_pOriginatingTask = pOriginatingTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::originator@ -- fetch the originator node from the job                  */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatorFetch(void)
{
  ENTRY(); // ENTRY("vJobOriginatorFetch"_Stz);
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  m_pDataStack->vPushInt(Job::iOriginatorNode(iJobId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.parentId@ -- fetch job parent id                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobParentIdFetch(void)
{
  ENTRY(); // ENTRY("vJobParentIdFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->iParentId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participant@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantFetch(void)
{
  ENTRY(); // ENTRY("vJobRemoveParticipant"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->iParticipantNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partcipant! -- store the participant node in the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantStore(void)
{
  ENTRY(); // ENTRY("vJobParticipantStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iParticipantNode = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vParticipantNode(iParticipantNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantHasError -- has any participant returned an error ?         */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantHasError(void)
{
  ENTRY(); // ENTRY("job.participantHasError"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 bHasError = false;
  i8 iResponses = pJob->m_pResponseVec->nItems();
  
  for (i8 i_Response = 0; i_Response < iResponses; i_Response++) {
    if (pJob->m_pResponseVec->pGet(i_Response)->m_eErrorCode != EC_NONE) {
      bHasError = true;
      break;
    }
  }
  m_pDataStack->vPushInt(bHasError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantResponse! -- store a job response for the participant       */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantResponseStore(void)
{
  ENTRY(); // ENTRY("vJobParticipantResponseStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
//  i8 iNode = m_pDataStack->iPopAsInt(__func__);
  
  Job* pParticipantJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // there should be a response vector
  RASSERT( pJob->m_pResponseVec );
  
  // create a new response
  Response* pResponse = new Response;
  
  // set node and error in the response
  //RASSERT(iNode == pParticipantJob->m_iParticipantNode);
  pResponse->m_iNode           = pParticipantJob->iParticipantNode();
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
  ENTRY(); // ENTRY("vJobPartitionsFetch"_Stz);
  
  Job_Sanity* pJob_Sanity = _pJob_Sanity(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob_Sanity->m_iPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase@ -- fetch the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(void)
{
  ENTRY(); // ENTRY("vJobPhaseFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase! -- store the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseStore(void)
{
  ENTRY(); // ENTRY("vJobPhaseStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobPhase = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vJobPhase((eJP)iJobPhase);
  
  //ActLog::vAdd(pJob,AN_JOB_PHASE,iJobPhase,_EG_JP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.postRecv -- run the post-receive function of the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobPostRecv(void)
{
  ENTRY(); // ENTRY("vJobPostRecv"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the job class on this node
  
  JobClass* aJobClass = nullptr;
  eCN eClass = pJob->eClass();
  
  if (CN_JOB_UNIVERSAL == eClass) {
    eClass = (eCN)pJob->pEnameSet()->iValue("<jobClass>"_Stz);
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
  ENTRY(); // ENTRY("vJobPreSend"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobRecv"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobRemoveOriginator"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobRemoveParticipant"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobRegionSizeFetch"_Stz);
  
  Job_Sanity* pJob_Sanity = _pJob_Sanity(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob_Sanity->m_iRegionSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.reseed -- reseed the job on the stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobReseed(void)
{
  ENTRY(); // ENTRY("vJobReseed"_Stz);
  
  i8 i_Seed = m_pDataStack->iPopInt(__func__);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobResponseFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushStr(pJob->m_xResponse.m_sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response! -- store the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseStore(void)
{
  ENTRY(); // ENTRY("vJobResponseStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s4K sResponse;
  m_pDataStack->vPopStr(sResponse,__func__);
  
  //pJob->m_xNode[iNode].m_sResponse = sResponse;
  pJob->m_xResponse.m_sResponse = sResponse;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid@ -- fetch the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidFetch(void)
{
  ENTRY(); // ENTRY("vJobRowsInvalidFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsInvalid = *pJob->m_xResponse.m_ixRowsInvalid;
  m_pDataStack->vPushInt(iRowsInvalid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid! -- store the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidStore(void)
{
  ENTRY(); // ENTRY("vJobRowsInvalidStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsInvalid = m_pDataStack->iPopInt(__func__);
  
  //pJob->m_xNode[iNode].m_ixRowsInvalid = iRowsInvalid;
  
  pJob->m_xResponse.m_ixRowsInvalid = iRowsInvalid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed@ -- fetch the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedFetch(void)
{
  ENTRY(); // ENTRY("vJobRowsProcessedFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsProcessed = *pJob->m_xResponse.m_ixRowsProcessed;
  m_pDataStack->vPushInt(iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed! -- store the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedStore(void)
{
  ENTRY(); // ENTRY("vJobRowsProcessedStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsProcessed = m_pDataStack->iPopInt(__func__);
  
  //pJob->m_xNode[iNode].m_ixRowsProcessed = iRowsProcessed;
  
  pJob->m_xResponse.m_ixRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped@ -- fetch the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedFetch(void)
{
  ENTRY(); // ENTRY("vJobRowsSkippedFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
 
  i8 iRowsSkipped = *pJob->m_xResponse.m_ixRowsSkipped;

  m_pDataStack->vPushInt(iRowsSkipped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped! -- store the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedStore(void)
{
  ENTRY(); // ENTRY("vJobRowsSkippedStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsSkipped = m_pDataStack->iPopInt(__func__);
  
  //pJob->m_xNode[iNode].m_ixRowsSkipped = iRowsSkipped;
  
  pJob->m_xResponse.m_ixRowsSkipped = iRowsSkipped;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seedDecode -- decode seed for received job                             */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedDecode(void)
{
  ENTRY(); // ENTRY("vJobSeedDecode"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  eSS eSeed = SS_SEED_RCV;
  
  if (JK_GENERIC == pJob->eJobKind()) {
    EnameSet* pEnameSet_ji = Job::pJobInfo_enameSet(pJob->szJobNm());
    
    s32 sSeed = pEnameSet_ji->szValue(C_TAG_SEEDRUN);
    
    eSeed = SS_SEED_RUN;
    
    vDebugPoint();
  }
  
  
  m_pDataStack->vPushInt(eSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seed@ -- fetch job seed                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedFetch(void)
{
  ENTRY(); // ENTRY("vJobSeedFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eSeed());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seed! -- seed the job                                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedStore(void)
{
  ENTRY(); // ENTRY("vJobSeedStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Seed = m_pDataStack->iPopInt(__func__);
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_ENDARGS);
  
  pJob->vSeed((eSS)i_Seed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.send -- pre-send of job                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSend(void)
{
  ENTRY(); // ENTRY("vJobSend"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobSessionStateFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iSessionId = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = nullptr;
  
  if (iSessionId != C_UNKNOWN)
    pSession = AM::pSessionArray()->pGetSession(iSessionId);
  
  m_pDataStack->vPushPtr(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.sessionState@ -- fetch job session state                               */
/*----------------------------------------------------------------------------*/
void Step::vJobSessionStateFetch(void)
{
  ENTRY(); // ENTRY("vJobSessionStateFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_xReqTuple.m_i_Session);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.show -- show the job                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobShow(void)
{
  ENTRY(); // ENTRY("vJobShow"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobSignalWaiter"_Stz);
  
  // there will be a thread waiting on completion of the job
  // this could be a prompt htread or a java thread via the connector
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pJob->bNoWaitTag()) {
    //! this is incomplete ... memory leak; job & task need to be deleted
    EXIT();
    return;
  }
  
  if (pJob->m_xReqTuple.m_iDetail)
    JobLog::vNote(pJob,"Connector/Prompt signalled by %s"_Stz,
                       aCN[pJob->eClass()]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT( _bIsValidJob(pJob->eClass()) );
  
  pJob->vJobState(JS_SIGNALLED);

  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);
  SessionArray::vMarkSignalled(pSession);
  RASSERT( _bIsValidJob(pJob->eClass()) );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pTask->vSchedulerAct(SA_SIGNAL);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.slot@ -- fetch job slot                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSlotFetch(void)
{
  ENTRY(); // ENTRY("vJobSessionSlotFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->i_Slot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn -- spawn the job on the stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawn(void)
{
  ENTRY(); // ENTRY("vJobSpawn"_Stz);
  

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_ENDARGS);

  
  IGNORE_RETURN(Api::bSpawnJob(pJob));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn_dedicated -- spawn the dedicated job on the stack                */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawnDedicated(void)
{
  ENTRY(); // ENTRY("vJobSpawnDedicated"_Stz);
  
  i8 i_Seed = m_pDataStack->iPopInt(__func__);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobSpawnUniversal"_Stz);
  
  s32 sJobNm;
  m_pDataStack->vPopStr(sJobNm,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get the ename set from the job info
  EnameSet* pEnameSet_jobInfo = Job::pJobInfo_enameSet(sJobNm);
  
  // find the job class
  JobClass* aJobClass = JobClass::pFind(CN_JOB_UNIVERSAL);

  // new the universal job
  ReqTuple xReqTuple("!!vJobSpawnUniversal"_Stz,false);
  Job_Universal* pJob_Universal = new Job_Universal(aJobClass,&xReqTuple);

  // ranslate job type and kind
  pJob_Universal->m_eJobType = (eJT) pEnameSet_jobInfo->iValue("<?jobType?>"_Stz);
  pJob_Universal->vJobKind((eJK) pEnameSet_jobInfo->iValue("<?jobKind?>"_Stz));

  // add the necessary ename tags
  EnameSet* pEnameSet_job = pJob_Universal->pEnameSet();
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
  ENTRY(); // ENTRY("vJobStateDiagram"_Stz);
  
  i8 iClassNo = m_pDataStack->iPopInt(__func__);

  s256 sDiagram;
  
  AM::vStateDiagram(sDiagram,(eCN)iClassNo);
  
  m_pDataStack->vPushStr(sDiagram);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.Task@ -- fetch the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskFetch(void)
{
  ENTRY(); // ENTRY("vJobTaskFetch"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->pTask());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.Task! -- store the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskStore(void)
{
  ENTRY(); // ENTRY("vJobTaskStore"_Stz);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY(); // ENTRY("vJobThis"_Stz);
  
  Job* pJob = m_pTask->m_pJob;
  
  m_pDataStack->vPushPtr(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::waitParticipant -- wait for participant till found in participant jobs*/
/*----------------------------------------------------------------------------*/
void Step::vJobWaitParticipant(void)
{
  ENTRY(); // ENTRY("vJobWaitParticipant"_Stz);
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  Job__Find* pFindJob = new Job__Find;
  pFindJob->m_iFocalId = iJobId;

  Job* pJobFound = nullptr;

  while ( ! pJobFound ) {
    AM::pParticipatingJobs()->vGrabLock();
    pJobFound = AM::pParticipatingJobs()->pFindJob(pFindJob);
    AM::pParticipatingJobs()->vDropLock();
  }

  delete pFindJob;
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
