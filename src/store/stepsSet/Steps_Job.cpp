/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Grid.hpp"
#include "Api.hpp"
#include "Session.hpp"
#include "Op.hpp"
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
  vAddStep("job::assertCorrectness"_Stz,  &Step::vJobAssertCorrectness);

  vAddStep("job.acksRemaining!"_Stz,   &Step::vJobAcksRemaining);
  vAddStep("job.assertBegin"_Stz,      &Step::vJobAssertBegin);
  vAddStep("job.assertNeutral"_Stz,    &Step::vJobAssertNeutral);
  vAddStep("job.assertOriginator"_Stz, &Step::vJobAssertOriginator);
  vAddStep("job.assertParticipant"_Stz,&Step::vJobAssertParticipant);
  vAddStep("job.assertReceived"_Stz,   &Step::vJobAssertReceived);

  vAddStep("job.class"_Stz,            &Step::vJobClass);
  
  vAddStep("job.dec2pcAcksRmg"_Stz,    &Step::vJobDec2pcAcksRmg);
  vAddStep("job.delete2pc"_Stz,        &Step::vJobDelete2pc);
  
  vAddStep("job.id@"_Stz,              &Step::vJobIdFetch_other);
  vAddStep("job.isExecuting"_Stz,      &Step::vJobIsExecuting);
  
  vAddStep("job.jobClass"_Stz,         &Step::vJobJobClass);
  vAddStep("job.jobList@"_Stz,         &Step::vJobJobListFetch);
  
  vAddStep("job.kind@"_Stz,            &Step::vJobKindFetch);
  
  vAddStep("job.name@"_Stz,            &Step::vJobNameFetch);
  vAddStep("job.noWait@"_Stz,          &Step::vJobNoWaitFetch);
  
  vAddStep("job::originator@"_Stz,     &Step::vJobOriginatorFetch);
  
  vAddStep("job.participantNode@"_Stz, &Step::vJobParticipantFetch_other);
  vAddStep("job.phase@"_Stz,           &Step::vJobPhaseFetch_other);
  vAddStep("job.printMember"_Stz,      &Step::vJobPrintMember);

  vAddStep("job.seedDecode"_Stz,       &Step::vJobSeedDecode);
  vAddStep("job.seed@"_Stz,            &Step::vJobSeedFetch);
  vAddStep("job.seed!"_Stz,            &Step::vJobSeedStore);
  vAddStep("job.session@"_Stz,         &Step::vJobSessionFetch);
  vAddStep("job.sessionState@"_Stz,    &Step::vJobSessionStateFetch);
  vAddStep("job.show"_Stz,             &Step::vJobShow);
  vAddStep("job.scheduleNextPhase"_Stz,&Step::vJobScheduleNextPhase);
  vAddStep("job.spawn"_Stz,            &Step::vJobSpawn);

  vAddStep("job::tagSet@"_Stz,         &Step::vJobTagSetFetch);
  vAddStep("job.task@"_Stz,            &Step::vJobTaskFetch_other);
  vAddStep("job.task!"_Stz,            &Step::vJobTaskStore_other);
  
  
  vAddStep("Job[]"_Stz,                  &Step::vRegJobGet);

  vAddStep("Job[].addDemon"_Stz,         &Step::vJobAddDemon);
  vAddStep("Job[].addLocal"_Stz,         &Step::vJobAddLocal);
  vAddStep("Job[].addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("Job[].addParticipant"_Stz,   &Step::vJobAddParticipant);
  vAddStep("Job[].addRetired"_Stz,       &Step::vJobAddRetired);
  
  vAddStep("Job[].error!"_Stz,           &Step::vJobErrorStore);
  vAddStep("Job[].error@"_Stz,           &Step::vJobErrorFetch);

  vAddStep("Job[].id@"_Stz,              &Step::vJobIdFetch);
  
  vAddStep("Job[].originatingId@"_Stz,   &Step::vJobOriginatingIdFetch);
  vAddStep("Job[].originatingId!"_Stz,   &Step::vJobOriginatingIdStore);
  
  vAddStep("Job[].participantNode@"_Stz, &Step::vJobParticipantFetch);
  vAddStep("Job[].participantNode!"_Stz, &Step::vJobParticipantStore);
  
  vAddStep("Job[].phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("Job[].phase!"_Stz,           &Step::vJobPhaseStore);
  
  vAddStep("Job[].removeLocal"_Stz,      &Step::vJobRemoveLocal);
  vAddStep("Job[].removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("Job[].removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("Job[].removeRetired"_Stz,    &Step::vJobRemoveRetired);
  
  vAddStep("Job[].seedWord@"_Stz,        &Step::vJobSeedWordFetch);
  vAddStep("Job[].seedWord!"_Stz,        &Step::vJobSeedWordStore);
  
  vAddStep("Job[].state@"_Stz,           &Step::vJobStateFetch);
  vAddStep("Job[].state!"_Stz,           &Step::vJobStateStore);

  vAddStep("Job[].task@"_Stz,            &Step::vJobTaskFetch);
  vAddStep("Job[].task!"_Stz,            &Step::vJobTaskStore);
  
  vAddStep("Job[].walk"_Stz,             &Step::vJobWalk);
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
/* Job[].addDemon -- add job to demon job set                                 */
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
/* Job[].addLocal -- add job to local job set                                 */
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
/* Job[].addOriginator -- add job to originator job set                       */
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
/* Job[].addParticipant -- add job to participant job set                     */
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
/* Job[].addRetired -- add job to retired job set                             */
/*----------------------------------------------------------------------------*/
void Step::vJobAddRetired(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
//  AM::pParticipatingJobs()->vGrabLock();
//  AM::pParticipatingJobs()->vAddJob(pJob);
//  AM::pParticipatingJobs()->vDropLock();
  
  AM::vJob_AddRetired(* m_pJob);

  //vTrace("%p ID %lld added to participant\n"_Stz, pJob, pJob->iJobId());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.assertBegin -- assert job is not in originator nor participant set     */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertBegin(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  

  if (Job::iNodeNo(pJob->iJobId()) == AM::pGrid()->iThisNodeNo()) {

    if (! pJob->bWasReceived())
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "job.assertOriginator: "
             "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    if (JL_NONE != pJob->eJobList())
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "job.assertBegin: "
             "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->iJobId());
    if (! pJobFound)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "job.assertBegin: "
             "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->iJobId());
    if (pJobFound)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "job.assertBegin: "
             "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  if (JL_NONE != pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertBegin: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->iJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertBegin: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->iJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertBegin: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::assertCorrectness -- assert correctness                               */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertCorrectness(void)
{
  ENTRY();
  
  //Job* pJob = m_pJob;
  
  //vTrace("job::assertCorrectness\n"_Stz);
    
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
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertNeutral: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->iJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertNeutral: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->iJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertNeutral: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

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
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_eJobList is not JL_ORIGINATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

  if (pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_bWasReceived is true [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->iJobId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

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
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertParticipant: "
           "Job::m_eJobList is not JL_PARTICIPATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->iJobId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertParticipant: "
           "Job not found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());

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
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "job.assertReceived: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeNo());
  
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
void Step::vJobDelete2pc(void)
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
/* job.dec2pcAcksRmg -- decrement acks remaining for this job                 */
/*----------------------------------------------------------------------------*/
void Step::vJobDec2pcAcksRmg(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 nAcksWereRemaining;
  bool bAmDecider = pJob->bAcksRmgDec(nAcksWereRemaining);
  
  m_pDataStack->vPushInt(bAmDecider);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].error@ -- fetch the job error                                        */
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
    pJob = m_pOp->pPipelineJob();
  }
  else {
    NEVER_REACH;
  }

  m_pDataStack->vPushInt(pJob->eJobError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].error! -- store the job error                                        */
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
/* Job[].id@ -- fetch the job id of the job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushInt(pJob->iJobId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->iJobId());
  
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
  
  m_pDataStack->vPushInt(pJob->bHasTag_noWait());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].originatingId@ -- fetch originating id from job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushInt(pJob->m_iOriginatingId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].originatingId! -- store originating id into job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingIdStore(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  i8 iOriginatingId = m_pDataStack->iPopInt(__func__);
  
  pJob->m_iOriginatingId = iOriginatingId;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::originator@ -- fetch the originator node from the job                 */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatorFetch(void)
{
  ENTRY();
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  m_pDataStack->vPushInt(Job::iOriginatorNode(iJobId));
  
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
/* Job[].phase@ -- fetch the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].phase@ -- fetch the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].phase! -- store the job phase                                        */
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
/* Job[].removeLocal -- remove job from local job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveLocal(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  bool bRemove = AM::bJob_RemoveLocal(pJob);
  
  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from local\n"_Stz,
           aCN[pJob->eGetClassNo()],pJob->iJobId());
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = AM::bJob_RemoveLocal(pJob,true);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].removeOriginator -- remove job from originator job set               */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveOriginator(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  bool bRemove = AM::bJob_RemoveOriginating(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from originator\n"_Stz,
           aCN[pJob->eGetClassNo()],pJob->iJobId());
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = AM::bJob_RemoveOriginating(pJob,true);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].removeParticipant -- remove job from participant job set             */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveParticipant(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  bool bRemove = AM::bJob_RemoveParticipating(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from participant\n"_Stz,
           aCN[pJob->eGetClassNo()],pJob->iJobId());
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = AM::bJob_RemoveParticipating(pJob,true);
  }
     
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].removeRetired -- remove job from retired job set                     */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveRetired(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;
  
  bool bRemove = AM::bJob_RemoveRetired(pJob);

  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from reited\n"_Stz,
           aCN[pJob->eGetClassNo()],pJob->iJobId());
    vTrace("------------------------------------\n"_Stz);
    
    bRemove = AM::bJob_RemoveRetired(pJob,true);
  }
     
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
    
  pJob->vSeed((eSS)i_Seed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].seedWord@ -- fetch job seed word                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedWordFetch(void)
{
  ENTRY();
 
  Job* pJob = * m_pJob;

  Word* pWord = pJob->pSeedWord();
  
  m_pDataStack->vPushPtr(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].seedWord! -- set job seed word                                         */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedWordStore(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
    
  pJob->vSeedWord(pWord);
  
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
  
  // example: this job is _execute and the jobRef is bootcluster (in ptp list)

  Job* pJobParticipant = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTaskParticipant = pJobParticipant->pTask();
  RASSERT(! pTaskParticipant);
  
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
/* Job[].phase@ -- fetch the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobStateFetch(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;

  m_pDataStack->vPushInt(pJob->eJobState());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].phase! -- store the job phase                                        */
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
/* job::tagSet@ -- push ptr to job TagSet                                     */
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
    pJob = m_pOp->pPipelineJob();
  }
  else {
    NEVER_REACH;
  }
  
  m_pDataStack->vPushPtr(pJob->pJobTags());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].task@ -- fetch the task executing the job                            */
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
/* Job[].task! -- store the task executing the job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskStore(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  Task* pTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  // for the moment this should always be null
  ASSERT(! pTask);
  pJob->vTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.task! -- store the task executing the job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskStore_other(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Task* pTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  // for the moment this should always be null
  ASSERT(! pTask);
  pJob->vTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job[].walk -- walk the job                                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobWalk(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  //pJob->vWalk(nullptr);
  
  auto fClosure = [](Object* pObject, i8 iDepth) {
    
    eCN  eClassNo  = pObject->eGetClassNo();
    Stz* szClassNm = aCN[eClassNo];

    vTrace("%sClosure: %p %s:%03lld\n"_Stz
           ,Stz::szTab(iDepth)
           ,pObject
           ,szClassNm
           ,eClassNo);
  };
  
  pJob->vWalkLambda(0,fClosure);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Job[].get -- get task                                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegJobGet(void)
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
    pJob = m_pOp->pPipelineJob();
  }
  else {
    NEVER_REACH;
  }
  
  m_pDataStack->vPushPtr(pJob);

  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
