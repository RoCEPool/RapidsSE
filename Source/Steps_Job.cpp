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

  vAddStep("job.acksRemaining!"_Stz,   &Step::vJobAcksRemaining);
  vAddStep("job.addDemon"_Stz,         &Step::vJobAddDemon);
  vAddStep("job.addLocal"_Stz,         &Step::vJobAddLocal);
  vAddStep("job.addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("job.addParticipant"_Stz,   &Step::vJobAddParticipant);
  vAddStep("job.addDemon"_Stz,         &Step::vJobAddDemon);
  vAddStep("job.assertBegin"_Stz,      &Step::vJobAssertBegin);
  vAddStep("job.assertNeutral"_Stz,    &Step::vJobAssertNeutral);
  vAddStep("job.assertOriginator"_Stz, &Step::vJobAssertOriginator);
  vAddStep("job.assertParticipant"_Stz,&Step::vJobAssertParticipant);
  vAddStep("job.assertReceived"_Stz,   &Step::vJobAssertReceived);

  vAddStep("job.bideOperand"_Stz,      &Step::vJobBideOperand);

  vAddStep("job.class"_Stz,            &Step::vJobClass);
  vAddStep("job.createBufferQueue"_Stz,&Step::vJobCreateBufferQueue);
  vAddStep("job.createResponseVec"_Stz,&Step::vJobCreateResponseVec);
  
  vAddStep("job.dec2pcAcksRmg"_Stz,    &Step::vJobDec2pcAcksRmg);
  vAddStep("job.delete"_Stz,           &Step::vJobDelete);
  
  vAddStep("job.tagSet@"_Stz,        &Step::vJobTagSetFetch);
  vAddStep("job.error!"_Stz,           &Step::vJobErrorStore);
  vAddStep("job.error@"_Stz,           &Step::vJobErrorFetch);

  vAddStep("job::findOriginator"_Stz,  &Step::vJobFindOriginator);
  vAddStep("job::findParticipant"_Stz, &Step::vJobFindParticipant);

  vAddStep("job.hasError"_Stz,         &Step::vJobHasError);

  vAddStep("job.id@"_Stz,              &Step::vJobIdFetch);
  vAddStep("job.IsExecuting"_Stz,      &Step::vJobIsExecuting);
  
  vAddStep("job.jobClass"_Stz,         &Step::vJobJobClass);
  vAddStep("job.jobList@"_Stz,         &Step::vJobJobListFetch);
  
  vAddStep("job.kind@"_Stz,            &Step::vJobKindFetch);
  
  vAddStep("job.name@"_Stz,            &Step::vJobNameFetch);
  vAddStep("job.nextOperand"_Stz,      &Step::vJobNextOperand);
  vAddStep("job.noWait@"_Stz,          &Step::vJobNoWaitFetch);
  
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
  vAddStep("job.phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("job.phase!"_Stz,           &Step::vJobPhaseStore);
  vAddStep("job.postRecv"_Stz,         &Step::vJobPostRecv);
  vAddStep("job.preSend"_Stz,          &Step::vJobPreSend);
  vAddStep("job.printMember"_Stz,      &Step::vJobPrintMember);

  vAddStep("job.removeLocal"_Stz,      &Step::vJobRemoveLocal);
  vAddStep("job.removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("job.removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("job.response@"_Stz,        &Step::vJobResponseFetch);
  vAddStep("job.response!"_Stz,        &Step::vJobResponseStore);
  vAddStep("job.returnPipeline"_Stz,   &Step::vJobReturnPipeline);
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
  vAddStep("job.signalSession"_Stz,    &Step::vJobSignalSession);
  vAddStep("job.slot@"_Stz,            &Step::vJobSlotFetch);
  vAddStep("job.scheduleNextPhase"_Stz, &Step::vJobScheduleNextPhase);
  vAddStep("job.spawn_received"_Stz,   &Step::vJobSpawnReceived);
  vAddStep("job::stateDiagram"_Stz,    &Step::vJobStateDiagram);
  
  vAddStep("job.task@"_Stz,            &Step::vJobTaskFetch);
  vAddStep("job.task!"_Stz,            &Step::vJobTaskStore);
}


/*----------------------------------------------------------------------------*/
/* job.acksRemaining! -- set acks remaining for this job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobAcksRemaining(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 nAcksRemaining = m_pDataStack->iPopInt(__func__);
  
  pJob->vAcksRmg(nAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addDemon -- add job to demon job set                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobAddDemon(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // add the job to the originating jobs
  //  AM::pOriginatingJobs()->vGrabLock();
  //  AM::pOriginatingJobs()->vAddJob(pJob);
  //  AM::pOriginatingJobs()->vDropLock();
  
  
  Scheduler::vJob_AddDemon(pJob);
  
  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Added to Demon List: %s %p"_Stz,
                  aCN[pJob->eClass()],
                  pJob);
    AM::pDemonJobs()->vLogJobs(pJob->m_iFocalId);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addLocal -- add job to local job set                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobAddLocal(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Scheduler::vJob_AddLocal(pJob);
  
  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,"Added to Local List: %s %p"_Stz,
                  aCN[pJob->eClass()],
                  pJob);
    AM::pLocalJobs()->vLogJobs(pJob->m_iFocalId);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addOriginator -- add job to originator job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobAddOriginator(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
    
  // add the job to the originating jobs
//  AM::pOriginatingJobs()->vGrabLock();
//  AM::pOriginatingJobs()->vAddJob(pJob);
//  AM::pOriginatingJobs()->vDropLock();
  

  Scheduler::vJob_AddOriginating(pJob);

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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
//  AM::pParticipatingJobs()->vGrabLock();
//  AM::pParticipatingJobs()->vAddJob(pJob);
//  AM::pParticipatingJobs()->vDropLock();
  
  Scheduler::vJob_AddParticipating(pJob);

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
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpOperandA = pJob->m_pOp->pPopQueue();
  
  if (m_pLogicUnit->m_xpOperandA.bIsNull()) {
    m_pTask->vScheduleBide();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClass(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = m_pTask->pJob();
  
  pJob->vJobState(JS_COMPLETED);
  
  m_pTask->vGrabTaskLock();
  m_pTask->vScheduleDelete();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.createBufferQueue -- create the buffer queue in the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobCreateBufferQueue(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  RASSERT( ! pJob->pBufferQueue() );
  pJob->vBufferQueue(new Queue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.printMember -- print a member of the job                               */
/*----------------------------------------------------------------------------*/
void Step::vJobPrintMember(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));  
  vTrace("Job: %p Class: %p\n"_Stz, pJob, aCN[pJob->eClass()]);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.createResponseVec -- create the response vector in the job             */
/*----------------------------------------------------------------------------*/
void Step::vJobCreateResponseVec(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  pJob->vOriginResponseVec(new ResponseVec);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.delete -- delete the job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobDelete(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  //vTrace("Deleting job %p\n"_Stz, pJob);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  
  delete pJob;
  EXIT();
}
  

/*----------------------------------------------------------------------------*/
/* job.dec2pcAcksRmg -- decrement acks remaining for this job                 */
/*----------------------------------------------------------------------------*/
void Step::vJobDec2pcAcksRmg(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  bool bAmDecider = pJob->bAcksRmgDec();
  
  m_pDataStack->vPushInt(bAmDecider);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.tagSet@ -- push ptr to job TagSet                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobTagSetFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->pJobTags());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error@ -- fetch the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->eJobError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error! -- store the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobError = m_pDataStack->iPopAsInt(__func__);
  
  //pJob->m_xNode[iNode].m_eErrorCode = (eEC)iJobError;
  pJob->vJobError((eEC)iJobError);
  
  //ActLog::vAdd(pJob,AN_ERROR_CODE,iJobError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findOriginator -- find job from originator job set                    */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(void)
{
  ENTRY();
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  Job* pJobFound = Scheduler::pJob_FindOriginating(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pLogicUnit->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findParticipant -- find job from participant job set                  */
/*----------------------------------------------------------------------------*/
void Step::vJobFindParticipant(void)
{
  ENTRY();
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
    
  Job* pJobFound = Scheduler::pJob_FindParticipating(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pLogicUnit->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasError -- return true if job has error                               */
/*----------------------------------------------------------------------------*/
void Step::vJobHasError(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->bJobHasError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_iFocalId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.isExecuting -- is the job executing (SEED_EXE)                         */
/*----------------------------------------------------------------------------*/
void Step::vJobIsExecuting(void)
{
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eJobList());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.kind@ -- fetch job kind                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobKindFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  
  m_pDataStack->vPushInt(pJob->eJobKind());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.name@ -- get name of job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobNameFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s32 szName = pJob->szJobNm();
  m_pDataStack->vPushStr(szName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.nextOperand -- get next operand                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobNextOperand(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* pOp = pJob->m_pOp;
  
  if (m_pLogicUnit->m_xpOperandA.bIsNull())
    m_pLogicUnit->m_xpOperandA = pOp->pPopQueue();
  ASSERT(*m_pLogicUnit->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.noWait@ -- fetch no-wait                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobNoWaitFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->bHasTag_noWait());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingId@ -- fetch originating id from job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_iOriginatingId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingId! -- store originating id into job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdStore(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->m_pOriginatingTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask! -- store the originating task in the job              */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskStore(void)
{
  ENTRY();
  
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
  ENTRY();
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  m_pDataStack->vPushInt(Job::iOriginatorNode(iJobId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.parentId@ -- fetch job parent id                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobParentIdFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->iParentId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participant@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->iParticipantNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partcipant! -- store the participant node in the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantStore(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 bHasError = false;
  i8 iResponses = pJob->pOriginResponseVec()->nItems();
  
  for (i8 i_Response = 0; i_Response < iResponses; i_Response++) {
    if (pJob->pOriginResponseVec()->pGet(i_Response)->m_eErrorCode != EC_NONE) {
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
//  i8 iNode = m_pDataStack->iPopAsInt(__func__);
  
  Job* pParticipantJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // there should be a response vector
  RASSERT( pJob->pOriginResponseVec() );
  
  // create a new response
  Response* pResponse = new Response;
  
  // set node and error in the response
  //RASSERT(iNode == pParticipantJob->m_iParticipantNode);
  pResponse->m_iNode           = pParticipantJob->iParticipantNode();
  pResponse->m_sResponse       = pParticipantJob->szResponse();
  pResponse->m_ixRowsProcessed = pParticipantJob->iRowsProcessed();
  pResponse->m_ixRowsInvalid   = pParticipantJob->iRowsInvalid();
  pResponse->m_ixRowsSkipped   = pParticipantJob->iRowsSkipped();
  pResponse->m_eErrorCode      = pParticipantJob->eJobError();
  
  if (pResponse->m_eErrorCode != EC_NONE)
    pJob->vParticipantHasError(true);
  
  // add the response to the vector of responses
  pJob->pOriginResponseVec()->vAdd(pResponse);
  
  //if (pResponse->m_eErrorCode != EC_NONE)
    //ActLog::vAdd(pJob,AN_ERROR_CODE,pResponse->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase@ -- fetch the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase! -- store the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseStore(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // run the post recv function
  Jobs::vJob_recv(pJob);
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  //ActLog::vAdd(pJob,AN_PRE_SEND,m_pTask->pJob());
  Jobs::vJob_send(pJob);
  
  if (pJob->m_xReqTuple.m_iDetail)
    JobLog::vNote(pJob,
                          "PreSend: %s %p"_Stz,
                          aCN[pJob->eClass()],
                          pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeLocal -- remove job from local job set                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveLocal(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  bool bRemove = Scheduler::bJob_RemoveLocal(pJob);
  
  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from local\n"_Stz,
           aCN[pJob->eClass()],pJob->m_iFocalId);
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = Scheduler::bJob_RemoveLocal(pJob,true);
  }
  
  if (pJob->m_xReqTuple.m_iDetail) {
    JobLog::vNote(pJob,
                  "Removed from Local List: %s %p"_Stz,
                  aCN[pJob->eClass()],
                  pJob);
    AM::pLocalJobs()->vLogJobs(pJob->m_iFocalId);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeOriginator -- remove job from originator job set                 */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveOriginator(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  bool bRemove = Scheduler::bJob_RemoveOriginating(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from originator\n"_Stz,
           aCN[pJob->eClass()],pJob->m_iFocalId);
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = Scheduler::bJob_RemoveOriginating(pJob,true);
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  bool bRemove = Scheduler::bJob_RemoveParticipating(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from participant\n"_Stz,
           aCN[pJob->eClass()],pJob->m_iFocalId);
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = Scheduler::bJob_RemoveParticipating(pJob,true);
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
/* job.response@ -- fetch the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s4K sResponse = pJob->szResponse();
  m_pDataStack->vPushStr(sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response! -- store the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s4K sResponse;
  m_pDataStack->vPopStr(sResponse,__func__);
  
  //pJob->m_xNode[iNode].m_sResponse = sResponse;
  pJob->vResponse(sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.returnPipeline -- queue a return piepline on outer waiting op          */
/*----------------------------------------------------------------------------*/
void Step::vJobReturnPipeline(void)
{
  ENTRY();
  
  NEVER_REACH;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid@ -- fetch the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsInvalid = pJob->iRowsInvalid();
  m_pDataStack->vPushInt(iRowsInvalid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid! -- store the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsInvalid = m_pDataStack->iPopInt(__func__);
  
  //pJob->m_xNode[iNode].m_ixRowsInvalid = iRowsInvalid;
  
  pJob->vRowsInvalid(iRowsInvalid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed@ -- fetch the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsProcessed = pJob->iRowsProcessed();
  m_pDataStack->vPushInt(iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed! -- store the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsProcessed = m_pDataStack->iPopInt(__func__);
  
  //pJob->m_xNode[iNode].m_ixRowsProcessed = iRowsProcessed;
  
  pJob->vRowsProcessed(iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped@ -- fetch the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
 
  i8 iRowsSkipped = pJob->iRowsSkipped();

  m_pDataStack->vPushInt(iRowsSkipped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped! -- store the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iRowsSkipped = m_pDataStack->iPopInt(__func__);
  
  //pJob->m_xNode[iNode].m_ixRowsSkipped = iRowsSkipped;
  
  pJob->vRowsSkipped(iRowsSkipped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seedDecode -- decode seed for received job                             */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedDecode(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  eSS eSeed = SS_SEED_RCV;
  
  if (JK_GENERIC == pJob->eJobKind()) {
    TagSet* pCmdTagSet = CmdHelper::pCmdTagSet(pJob->szJobNm());
    
    s32 sSeed = pCmdTagSet->szValue(C_TAG_SEEDRUN);
    
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->eSeed());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seed! -- seed the job                                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Seed = m_pDataStack->iPopInt(__func__);
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_VARGS_END);
  
  pJob->vSeed((eSS)i_Seed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.send -- pre-send of job                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSend(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // call the job pre-send function
  Jobs::vJob_send(pJob);  // come back to this, remove session as parameter to delete
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.session@ -- fetch job session                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobSessionFetch(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_xReqTuple.m_i_Session);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.show -- show the job                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobShow(void)
{
  ENTRY();
  
  RASSERT(0);
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.signalSession -- signal the waiting session                         */
/*----------------------------------------------------------------------------*/
void Step::vJobSignalSession(void)
{
  ENTRY();
  
  // there will be a thread waiting on completion of the job
  // this could be a prompt thread or a java thread via the connector
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pJob->bHasTag_noWait()) {
    //! this is incomplete ... memory leak; job & task need to be deleted
    EXIT();
    return;
  }
  
  if (pJob->m_xReqTuple.m_iDetail)
    JobLog::vNote(pJob,"Connector/Prompt signalled by %s"_Stz,
                       aCN[pJob->eClass()]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pTask->vGrabTaskLock();

  RASSERT( _bIsValidJob(pJob->eClass()) );
  
  pJob->vJobState(JS_SIGNALLED);

  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);
  SessionArray::vMarkSignalled(pSession);
  RASSERT( _bIsValidJob(pJob->eClass()) );
  

  
  m_pTask->vJob(nullptr);

  pSession->m_oSsnMutex.vLock(m_pTask);
  pSession->m_oSsnMutex.vSignal(m_pTask);
  pSession->m_oSsnMutex.vUnlock(m_pTask);

  m_pTask->vDropTaskLock();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //m_pTask->vScheduleCompletion();
  

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.slot@ -- fetch job slot                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSlotFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->i_Slot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.scheduleNextPhase -- schedule next phase for the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobScheduleNextPhase(void)
{
  ENTRY();
  
  // example: this job is _execute and the jobRef is bootcluster (in ptp list)

  Job* pJobParticipant = _pJob(m_pDataStack->pPopPtr(__func__));
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJobParticipant->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_VARGS_END);

  Task* pTaskParticipant = pJobParticipant->pTask();
  RASSERT(! pTaskParticipant);
  
  AM::vScheduleNextPhase(pJobParticipant,m_pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn -- spawn the job on the stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawnReceived(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_SPAWN_JOB,
              "%s spawned by %s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()],
              aCN[m_pTask->m_pJob->eClass()],
              C_VARGS_END);

  Task* pTask = pJob->pTask();
  RASSERT(! pTask);
//  if ( pTask) {
//    vTrace("****Job: %s has a task in job.spawn\n"_Stz,pJob->szJobNm());
//    RASSERT(QK_NONE == pTask->eQueue());
//    RASSERT(TR_NONE == pTask->eTranstionRequest());
//  }
  
  Api::vSpawnJob_received(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.stateDiagram -- generate state diagram for given job class             */
/*----------------------------------------------------------------------------*/
void Step::vJobStateDiagram(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->pTask());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.Task! -- store the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  // for the moment this should always be null
  ASSERT(! pTask);
  pJob->vTask(pTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

