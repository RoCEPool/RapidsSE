/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Grid.hpp"
#include "Api.hpp"
#include "Session.hpp"
#include "Stream.hpp"
#include "Op.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "PipeWork.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "JobList.hpp"
#include "Sessions.hpp"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Job(void)
{
  vAddStep("job.class"_Stz,            &Step::vJobClass);
  
  vAddStep("job.delete"_Stz,           &Step::vJobDelete);
  
  vAddStep("job.entry@"_Stz,           &Step::vJobEntryFetch_other);
  vAddStep("job.entry!"_Stz,           &Step::vJobEntryStore_other);
  
  vAddStep("job.id@"_Stz,              &Step::vJobIdFetch_other);
  
  vAddStep("job.jobClass"_Stz,         &Step::vJobJobClass);
  vAddStep("job.jobList@"_Stz,         &Step::vJobJobListFetch);
  
  vAddStep("job.kind@"_Stz,            &Step::vJobKindFetch);
  
  vAddStep("job.name@"_Stz,            &Step::vJobNameFetch);
  vAddStep("job.noWait@"_Stz,          &Step::vJobNoWaitFetch);
  
  vAddStep("job.participantNode@"_Stz, &Step::vJobParticipantFetch_other);
  vAddStep("job.phase@"_Stz,           &Step::vJobPhaseFetch_other);
  vAddStep("job.pipeToOp0"_Stz,        &Step::vJobPipeToOp0);
  vAddStep("job.printMember"_Stz,      &Step::vJobPrintMember);

  vAddStep("job.reapable@"_Stz,        &Step::vJobReapableFetch_other);
  vAddStep("job.reapable!"_Stz,        &Step::vJobReapableStore_other);
  
  vAddStep("job.session@"_Stz,         &Step::vJobSessionFetch);
  vAddStep("job.sessionState@"_Stz,    &Step::vJobSessionStateFetch);
  vAddStep("job.show"_Stz,             &Step::vJobShow);
  vAddStep("job.scheduleNextPhase"_Stz,&Step::vJobScheduleNextPhase);
  vAddStep("job.spawn"_Stz,            &Step::vJobSpawn);

  vAddStep("job.task@"_Stz,            &Step::vJobTaskFetch_other);
  
  
  vAddStep("[job]"_Stz,                  &Step::vJobGet);

  vAddStep("[job].2pcDecider!"_Stz,      &Step::vJob2pcDeciderStore);
  vAddStep("[job].addDemon"_Stz,         &Step::vJobAddDemon);
  vAddStep("[job].addLocal"_Stz,         &Step::vJobAddLocal);
  vAddStep("[job].addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("[job].addParticipant"_Stz,   &Step::vJobAddParticipant);
  vAddStep("[job].addRetired"_Stz,       &Step::vJobAddRetired);
  vAddStep("[job].assertBegin"_Stz,      &Step::vJobAssertBegin);
  vAddStep("[job].assertOrdinary"_Stz,   &Step::vJobAssertOrdinary);
  vAddStep("[job].assertOriginator"_Stz, &Step::vJobAssertOriginator);
  vAddStep("[job].assertParticipant"_Stz,&Step::vJobAssertParticipant);
  vAddStep("[job].assertReceived"_Stz,   &Step::vJobAssertReceived);
  vAddStep("[job].assertStructure"_Stz,  &Step::vJobAssertStructure);

  vAddStep("[job].entryPoint@"_Stz,      &Step::vJobEntryPointFetch);
  vAddStep("[job].entryPoint!"_Stz,      &Step::vJobEntryPointStore);

  vAddStep("[job].error@"_Stz,           &Step::vJobErrorFetch);
  vAddStep("[job].error!"_Stz,           &Step::vJobErrorStore);

  vAddStep("[job].id@"_Stz,              &Step::vJobIdFetch);
    
  vAddStep("[job].originNodeNo@"_Stz,    &Step::vJobOriginNodeNoFetch);
  
  vAddStep("[job].participantNodeNo@"_Stz, &Step::vJobParticipantFetch);
  vAddStep("[job].participantNodeNo!"_Stz, &Step::vJobParticipantStore);
  vAddStep("[job].preExecuteTest"_Stz,   &Step::vJobPreExecuteTest);
  vAddStep("[job].phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("[job].phase!"_Stz,           &Step::vJobPhaseStore);
  vAddStep("[job].progress"_Stz,         &Step::vJobProgress);
  vAddStep("[job].progressReset"_Stz,    &Step::vJobProgressReset);
  vAddStep("[job].progressSeconds"_Stz,  &Step::vJobProgressSeconds);

  vAddStep("[job].reapable@"_Stz,        &Step::vJobReapableFetch);
  vAddStep("[job].reapable!"_Stz,        &Step::vJobReapableStore);
  vAddStep("[job].removeLocal"_Stz,      &Step::vJobRemoveLocal);
  vAddStep("[job].removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("[job].removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("[job].removeRetired"_Stz,    &Step::vJobRemoveRetired);
    
  vAddStep("[job].state@"_Stz,           &Step::vJobStateFetch);
  vAddStep("[job].state!"_Stz,           &Step::vJobStateStore);

  vAddStep("[job].tagSet@"_Stz,          &Step::vJobTagSetFetch);
  vAddStep("[job].task@"_Stz,            &Step::vJobTaskFetch);
  vAddStep("[job].tscBeginning"_Stz,     &Step::vJobTscBeginning);
  vAddStep("[job].tscBegun"_Stz,         &Step::vJobTscBegun);
  vAddStep("[job].tscExecuting"_Stz,     &Step::vJobTscExecuting);
  vAddStep("[job].tscExecuted"_Stz,      &Step::vJobTscExecuted);
  vAddStep("[job].tscEnding"_Stz,        &Step::vJobTscEnding);
  vAddStep("[job].tscEnded"_Stz,         &Step::vJobTscEnded);
  vAddStep("[job].tscCanceling"_Stz,     &Step::vJobTscCanceling);
  vAddStep("[job].tscCanceled"_Stz,      &Step::vJobTscCanceled);
  vAddStep("[job].tscParticipant"_Stz,   &Step::vJobTscParticipant);
}


/*----------------------------------------------------------------------------*/
/* job.2pcDecider! -- set acks remaining for this job                         */
/*----------------------------------------------------------------------------*/
void Step::vJob2pcDeciderStore(void)
{
  ENTRY();
    
  i8 nAcksRemaining = m_pDataStack->iPopInt(__func__);
  
  m_pJob->v2pcDeciderRmg(nAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].addDemon -- add job to demon job set                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobAddDemon(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // add the job to the originating jobs
  //  AM::pOriginatingJobs()->vGrabLock();
  //  AM::pOriginatingJobs()->vAddJob(pJob);
  //  AM::pOriginatingJobs()->vDropLock();
  
  
  AM::vJob_AddDemon(* m_pJob);
  
  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].addLocal -- add job to local job set                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobAddLocal(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  AM::vJob_AddLocal(* m_pJob);
  
  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].addOriginator -- add job to originator job set                       */
/*----------------------------------------------------------------------------*/
void Step::vJobAddOriginator(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
    
  // add the job to the originating jobs
//  AM::pOriginatingJobs()->vGrabLock();
//  AM::pOriginatingJobs()->vAddJob(pJob);
//  AM::pOriginatingJobs()->vDropLock();
  

  AM::vJob_AddOriginating(* m_pJob);

  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].addParticipant -- add job to participant job set                     */
/*----------------------------------------------------------------------------*/
void Step::vJobAddParticipant(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
//  AM::pParticipatingJobs()->vGrabLock();
//  AM::pParticipatingJobs()->vAddJob(pJob);
//  AM::pParticipatingJobs()->vDropLock();
  
  AM::vJob_AddParticipating(* m_pJob);

  //vTrace("%p ID %lld added to participant\n"_Stz, pJob, pJob->iJobId());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].addRetired -- add job to retired job set                             */
/*----------------------------------------------------------------------------*/
void Step::vJobAddRetired(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
//  AM::pParticipatingJobs()->vGrabLock();
//  AM::pParticipatingJobs()->vAddJob(pJob);
//  AM::pParticipatingJobs()->vDropLock();
  
  Job* pJob = * m_pJob;
  
  RASSERT(pJob->pTask());
  
//  vTrace("Addingd Job %p ID %lld to retired list\n"_Stz,
//         pJob,
//         pJob->iJobId() & 0xFFFFFFFF
//         );

  AM::vJob_AddRetired(pJob);

    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertBegin -- assert job is not in originator nor participant set     */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertBegin(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  

  if ((i8)pJob->tJobId().uNodeNo() == AM::pGrid()->iThisNodeNo()) {

    if (! pJob->bWasReceived())
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertOriginator: "
             "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    if (JL_NONE != pJob->eJobList())
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertBegin: "
             "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tJobId());
    if (! pJobFound)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertBegin: "
             "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tJobId());
    if (pJobFound)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertBegin: "
             "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  if (JL_NONE != pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertOrdinary -- assert job is not in originator nor participant set   */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertOrdinary(void)
{
  ENTRY();
    
  if (JL_NONE != m_pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOrdinary: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(m_pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOrdinary: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

  pJobFound = AM::pParticipatingJobs()->pFindJob(m_pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOrdinary: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertOriginator -- assert job is in originator job set                */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertOriginator(void)
{
  ENTRY();
    
  if (JL_ORIGINATING != m_pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_eJobList is not JL_ORIGINATING [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

  if (m_pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_bWasReceived is true [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(m_pJob->tJobId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertParticipant -- assert job is in participant job set              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertParticipant(void)
{
  ENTRY();
    
  if (JL_PARTICIPATING != m_pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job::m_eJobList is not JL_PARTICIPATING [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  if (! m_pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(m_pJob->tJobId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job not found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertReceived -- assert job was received                              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertReceived(void)
{
  ENTRY();
    
  if (! m_pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertReceived: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertStructure -- assert structural correctness                     */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertStructure(void)
{
  ENTRY();
  
  //Job* pJob = m_pJob;
  
  //vTrace("[job].assertStructure\n"_Stz);
  
  RASSERT(m_pJob.bIsNotNull());
  RASSERT(m_pJob->pTask() == this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClass(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  eCN eClass = pJob->eGetClassNo();
  
  m_pDataStack->vPushInt(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.printMember -- print a member of the job                               */
/*----------------------------------------------------------------------------*/
void Step::vJobPrintMember(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));  
  vTrace("Job: %p Class: %p\n"_Stz, pJob, aCN[pJob->eGetClassNo()]);
  
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
  eCN eClass = pJob->eGetClassNo();
  UNUSED(eClass);

  pJob->vGuardedDelete(CN_JOB,&pJob,
                       CN_STEP,this,(Stz*)__func__);

  EXIT();
}
  

/*----------------------------------------------------------------------------*/
/* [job].entryPoint@ -- fetch the job sen entry code                               */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryPointFetch(void)
{
  ENTRY();
  
  Job* pJob = nullptr;

  if (m_pJob.bIsNotNull()) {
    //RASSERT(! *m_pOp);
    pJob = *m_pJob;
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob.bIsNull());
    pJob = m_pOp->pJobOfOp();
  }
  else {
    NEVER_REACH;
  }

  m_pDataStack->vPushStz(pJob->szEntryPoint());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.entry@ -- fetch the job sen entry code                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushStz(pJob->szEntryPoint());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].entryPoint! -- store the job entry code                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryPointStore(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  s4K sEntry;
  m_pDataStack->vPopStr(sEntry,__func__);
  
  pJob->vEntryPoint(sEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.entry! -- store the job entry code                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryStore_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  s4K sEntry;
  m_pDataStack->vPopStr(sEntry,__func__);
  
  pJob->vEntryPoint(sEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].error@ -- fetch the job error                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorFetch(void)
{
  ENTRY();
  
  Job* pJob = nullptr;

  if (m_pJob.bIsNotNull()) {
    //RASSERT(! *m_pOp);
    pJob = *m_pJob;
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob.bIsNull());
    pJob = m_pOp->pJobOfOp();
  }
  else {
    NEVER_REACH;
  }

  m_pDataStack->vPushInt(pJob->eJobError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].error! -- store the job error                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorStore(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  i8 iJobError = m_pDataStack->iPopAsInt(__func__);
  
  //pJob->m_xNode[iNode].m_eErrorCode = (eEC)iJobError;
  pJob->vJobError((eEC)iJobError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].id@ -- fetch the job id of the job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->tJobId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->tJobId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].get -- get task                                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobGet(void)
{
  ENTRY();
  
  // revised job/op model

  Job* pJob = nullptr;
  
  if (m_pJob.bIsNotNull()) {
    //RASSERT(! *m_pOp);
    pJob = *m_pJob;
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob.bIsNull());
    pJob = m_pOp->pJobOfOp();
  }
  else {
    NEVER_REACH;
  }
  
  m_pDataStack->vPushPtr(pJob);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobClass -- get the JobClass of the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobJobClass(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobClass_p = (i8)pJob->eGetClassNo();
  
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
/* job.noWait@ -- fetch no-wait                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobNoWaitFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->bHasTag(C_TAG_NOWAIT));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].originNodeNo@ -- fetch the originator node no from this job          */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginNodeNoFetch(void)
{
  ENTRY();
  
  JobId tJobId = m_pJob->tJobId();
  
  i8 iNodeNo = (i8)tJobId.uNodeNo();
  
  m_pDataStack->vPushInt(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantNode@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushInt(pJob->iParticipantNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participantNode@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantFetch_other(void)
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
  
  Job* pJob = * m_pJob;

  i8 iParticipantNode = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vParticipantNode(iParticipantNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.preExecuteTest -- test if job can be run                               */
/*----------------------------------------------------------------------------*/
void Step::vJobPreExecuteTest(void)
{
  ENTRY();
  
  eEC eError = AM::JOB::ePreExecuteTest(m_pJob);
  
  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].phase@ -- fetch the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].phase@ -- fetch the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.pipeToOp0 -- append operand onto op0 of given job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobPipeToOp0(void)
{
  ENTRY();

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // !!!!!!!!!!!!!!
  // move to AM

  Pipeline* pPipeline = pJob->pPipelineX();
  PipeWork* pPipeWork = pPipeline->pPipeWork();

  Stage*    pStage0   = pPipeWork->pGet(0);
  Stream*   pStand0   = pStage0->pGet(0);
  Op*       pOp0      = pStand0->pGet(0);

  pOp0->vAppendLeft(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].phase! -- store the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseStore(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  i8 iJobPhase = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vJobPhase((eJP)iJobPhase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].progress -- progress print                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobProgress(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  Timer* pProgress = pJob->pProgressTimer();

  pProgress->vUpdate();
  
  s4K sPrint;
  m_pDataStack->vPopStr(sPrint,__func__);
  
  vTrace("%s %lldus\n"_Stz
         ,sPrint.szStz()
         ,pProgress->iMicroseconds())

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].progressReset -- progress reset                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobProgressReset(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  Timer* pProgress = pJob->pProgressTimer();

  pProgress->vReset();
  pProgress->vStart();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].progressSeconds -- get progress in seconds                           */
/*----------------------------------------------------------------------------*/
void Step::vJobProgressSeconds(void)
{
  ENTRY();
  
  Job*  pJob = MY::pJob();
    
  Timer* pProgress = pJob->pProgressTimer();

  pProgress->vUpdate();
  
  i8 iSeconds = pProgress->iSeconds();

  m_pDataStack->vPushInt(iSeconds);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [job].reapable@ -- fetch the job reapable flag                             */
/*----------------------------------------------------------------------------*/
void Step::vJobReapableFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushInt(pJob->bReapable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.reapable@ -- fetch the job reapable flag                               */
/*----------------------------------------------------------------------------*/
void Step::vJobReapableFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->bReapable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].reapable! -- store the job reapable flag                             */
/*----------------------------------------------------------------------------*/
void Step::vJobReapableStore(void)
{
  ENTRY();
  
  if (m_pJob.bIsNull())
    AM::SE::vWhereAmI();
  
  Job* pJob = * m_pJob;

  i8 iJobReapable = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vReapable(iJobReapable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.reapable! -- store the job reapable flag                               */
/*----------------------------------------------------------------------------*/
void Step::vJobReapableStore_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iJobReapable = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vReapable(iJobReapable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeLocal -- remove job from local job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveLocal(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  AM::vJob_RemoveLocal(pJob);
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeOriginator -- remove job from originator job set               */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveOriginator(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  AM::vJob_RemoveOriginating(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeParticipant -- remove job from participant job set             */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveParticipant(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  AM::vJob_RemoveParticipating(pJob);
     
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeRetired -- remove job from retired job set                     */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveRetired(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  AM::vJob_RemoveRetired(pJob);
     
  EXIT();
}
  
/*----------------------------------------------------------------------------*/
/* job.session@ -- fetch job session                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobSessionFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 iSessionId = pJob->m_i_Session;
  
  Session* pSession = nullptr;
  
  if (iSessionId != C_UNKNOWN)
    pSession = AM::pSessions()->pGetSession(iSessionId);
  
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
  
  m_pDataStack->vPushInt(pJob->m_i_Session);
  
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
  UNUSED(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.scheduleNextPhase -- schedule next phase for the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobScheduleNextPhase(void)
{
  ENTRY();
    
  Job* pJobParticipant = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTaskParticipant = pJobParticipant->pTask();
  RASSERT(pTaskParticipant);
  
  AM::vScheduleNextPhase(pJobParticipant,this);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn -- spawn the job on the stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawn(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTask = pJob->pTask();
  RASSERT(! pTask);
//  if ( pTask) {
//    vTrace("****Job: %s has a task in job.spawn\n"_Stz,pJob->szJobNm());
//    RASSERT(QK_NONE == pTask->eQueue());
//    RASSERT(TR_NONE == pTask->eTransition());
//  }
  
  Api::vSpawnTask_receivedJob(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].phase@ -- fetch the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobStateFetch(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;

  m_pDataStack->vPushInt(pJob->eJobState());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].phase! -- store the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobStateStore(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;

  i8 iJobState = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vJobState((eJS)iJobState);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [job].tagSet@ -- push ptr to job TagSet                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobTagSetFetch(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJob = nullptr;

  if (m_pJob.bIsNotNull()) {
    //RASSERT(! *m_pOp);
    pJob = *m_pJob;
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob.bIsNull());
    pJob = m_pOp->pJobOfOp();
  }
  else {
    NEVER_REACH;
  }
  
  m_pDataStack->vPushPtr(pJob->pJobTags());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].task@ -- fetch the task executing the job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushPtr(pJob->pTask());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.task@ -- fetch the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushPtr(pJob->pTask());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscBeginning -- set tsc for beginning                                */
/*----------------------------------------------------------------------------*/
void Step::vJobTscBeginning(void)
{
  ENTRY();
  
  m_pJob->vTscBeginning(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscBegun -- set tsc for begun                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobTscBegun(void)
{
  ENTRY();
  
  m_pJob->vTscBegun(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscExecuting -- set tsc for executing                                */
/*----------------------------------------------------------------------------*/
void Step::vJobTscExecuting(void)
{
  ENTRY();
  
  m_pJob->vTscExecuting(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscExecuted -- set tsc for executed                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobTscExecuted(void)
{
  ENTRY();
  
  m_pJob->vTscExecuted(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscEnding -- set tsc for ending                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobTscEnding(void)
{
  ENTRY();
  
  m_pJob->vTscEnding(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscEnded -- set tsc for ended                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobTscEnded(void)
{
  ENTRY();
  
  m_pJob->vTscEnded(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscCanceling -- set tsc for canceling                                */
/*----------------------------------------------------------------------------*/
void Step::vJobTscCanceling(void)
{
  ENTRY();
  
  m_pJob->vTscCanceling(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscCanceled -- set tsc for canceled                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobTscCanceled(void)
{
  ENTRY();
  
  m_pJob->vTscCanceled(Hardware::iReadTsc());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscParticipant -- set tsc for participant                            */
/*----------------------------------------------------------------------------*/
void Step::vJobTscParticipant(void)
{
  ENTRY();
  
  m_pJob->vTscParticipant(Hardware::iReadTsc());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
