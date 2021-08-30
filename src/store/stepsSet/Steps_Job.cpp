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
  vAddStep("job::retire"_Stz,             &Step::vJobRetire);
  vAddStep("job::state@"_Stz,             &Step::vJobStateFetch);
  vAddStep("job::state!"_Stz,             &Step::vJobStateStore);

  vAddStep("job.acksRemaining!"_Stz,   &Step::vJobAcksRemaining);
  vAddStep("job.addDemon"_Stz,         &Step::vJobAddDemon);
  vAddStep("job.addLocal"_Stz,         &Step::vJobAddLocal);
  vAddStep("job.addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("job.addParticipant"_Stz,   &Step::vJobAddParticipant);
  vAddStep("job.addRetired"_Stz,       &Step::vJobAddRetired);
  vAddStep("job.assertBegin"_Stz,      &Step::vJobAssertBegin);
  vAddStep("job.assertNeutral"_Stz,    &Step::vJobAssertNeutral);
  vAddStep("job.assertOriginator"_Stz, &Step::vJobAssertOriginator);
  vAddStep("job.assertParticipant"_Stz,&Step::vJobAssertParticipant);
  vAddStep("job.assertReceived"_Stz,   &Step::vJobAssertReceived);

  vAddStep("job.class"_Stz,            &Step::vJobClass);
  
  vAddStep("job.dec2pcAcksRmg"_Stz,    &Step::vJobDec2pcAcksRmg);
  vAddStep("job.delete"_Stz,           &Step::vJobDelete);
  
  vAddStep("job.error!"_Stz,           &Step::vJobErrorStore);
  vAddStep("job.error@"_Stz,           &Step::vJobErrorFetch);

  vAddStep("job::findOriginator"_Stz,  &Step::vJobFindOriginator);
  vAddStep("job::findParticipant"_Stz, &Step::vJobFindParticipant);

  vAddStep("job.hasError"_Stz,         &Step::vJobHasError);

  vAddStep("job.id@"_Stz,              &Step::vJobIdFetch);
  vAddStep("job.isExecuting"_Stz,      &Step::vJobIsExecuting);
  
  vAddStep("job.jobClass"_Stz,         &Step::vJobJobClass);
  vAddStep("job.jobList@"_Stz,         &Step::vJobJobListFetch);
  
  vAddStep("job.kind@"_Stz,            &Step::vJobKindFetch);
  
  vAddStep("job.name@"_Stz,            &Step::vJobNameFetch);
  vAddStep("job.noWait@"_Stz,          &Step::vJobNoWaitFetch);
  
  vAddStep("job.originatingId@"_Stz,   &Step::vJobOriginatingIdFetch);
  vAddStep("job.originatingId!"_Stz,   &Step::vJobOriginatingIdStore);
  vAddStep("job.originatingTask@"_Stz, &Step::vJobOriginatingTaskFetch);
  vAddStep("job.originatingTask!"_Stz, &Step::vJobOriginatingTaskStore);
  vAddStep("job::originator@"_Stz,     &Step::vJobOriginatorFetch);
  
  vAddStep("job.participant@"_Stz,     &Step::vJobParticipantFetch);
  vAddStep("job.participant!"_Stz,     &Step::vJobParticipantStore);
  vAddStep("job.phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("job.phase!"_Stz,           &Step::vJobPhaseStore);
  vAddStep("job.printMember"_Stz,      &Step::vJobPrintMember);

  vAddStep("job.removeLocal"_Stz,      &Step::vJobRemoveLocal);
  vAddStep("job.removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("job.removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("job.removeRetired"_Stz,    &Step::vJobRemoveRetired);

  vAddStep("job.seedDecode"_Stz,       &Step::vJobSeedDecode);
  vAddStep("job.seed@"_Stz,            &Step::vJobSeedFetch);
  vAddStep("job.seed!"_Stz,            &Step::vJobSeedStore);
  vAddStep("job.seedWord@"_Stz,        &Step::vJobSeedWordFetch);
  vAddStep("job.seedWord!"_Stz,        &Step::vJobSeedWordStore);
  vAddStep("job.session@"_Stz,         &Step::vJobSessionFetch);
  vAddStep("job.sessionState@"_Stz,    &Step::vJobSessionStateFetch);
  vAddStep("job.show"_Stz,             &Step::vJobShow);
  vAddStep("job.signalSession"_Stz,    &Step::vJobSignalSession);
  vAddStep("job.singularOp@"_Stz,      &Step::vJobSingularOpFetch);
  vAddStep("job.singularOp!"_Stz,      &Step::vJobSingularOpStore);
  vAddStep("job.scheduleNextPhase"_Stz,&Step::vJobScheduleNextPhase);
  vAddStep("job.spawn"_Stz,            &Step::vJobSpawn);

  vAddStep("job::tagSet@"_Stz,         &Step::vJobTagSetFetch);
  vAddStep("job.task@"_Stz,            &Step::vJobTaskFetch);
  vAddStep("job.task!"_Stz,            &Step::vJobTaskStore);
  
  
  vAddStep("job[]"_Stz,      &Step::vRegJobGet);

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
  
  
  AM::vJob_AddDemon(pJob);
  
  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addLocal -- add job to local job set                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobAddLocal(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  AM::vJob_AddLocal(pJob);
  
  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
    
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
  

  AM::vJob_AddOriginating(pJob);

  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
      
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
  
  AM::vJob_AddParticipating(pJob);

  //vTrace("%p ID %lld added to participant\n"_Stz, pJob, pJob->iJobId());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addRetired -- add job to retired job set                               */
/*----------------------------------------------------------------------------*/
void Step::vJobAddRetired(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
//  AM::pParticipatingJobs()->vGrabLock();
//  AM::pParticipatingJobs()->vAddJob(pJob);
//  AM::pParticipatingJobs()->vDropLock();
  
  AM::vJob_AddRetired(pJob);

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
void Step::vJobDelete(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  //vTrace("Deleting job %p\n"_Stz, pJob);
  
  // find the job in the catalog
  eCN eClass = pJob->eGetClassNo();
  UNUSED(eClass);

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
  
  i8 nAcksWereRemaining;
  bool bAmDecider = pJob->bAcksRmgDec(nAcksWereRemaining);
  
  m_pDataStack->vPushInt(bAmDecider);
  
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
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findOriginator -- find job from originator job set                    */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(void)
{
  ENTRY();
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  Job* pJobFound = AM::pJob_FindOriginating(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_p_TaskLogicUnit->m_xpJobRef);
  
  
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
    
  Job* pJobFound = AM::pJob_FindParticipating(iJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_p_TaskLogicUnit->m_xpJobRef);
  
  
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
  
  m_pDataStack->vPushInt(pJob->iJobId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.singularOp@ -- fetch the input op                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobSingularOpFetch(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->pSingularOp());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.singularOp! -- store the input op                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobSingularOpStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Op* pOp = _pOp(m_pDataStack->pPopPtr(__func__));

  pJob->vSingularOp(pOp);
  
  // get the job in the session
  Job* pJob_Originating = pOp->pPipelineJob()->m__dpOriginatingTask->pJob();

  pJob_Originating->vSingularOp(pOp);

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
  
  m_pDataStack->vPushPtr(pJob->m__dpOriginatingTask);
  
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
  ASSERT(CN_TASK == pOriginatingTask->eGetClassNo());
  
  pJob->m__dpOriginatingTask = pOriginatingTask;
  
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
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeLocal -- remove job from local job set                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveLocal(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
/* job.removeOriginator -- remove job from originator job set                 */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveOriginator(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
/* job.removeParticipant -- remove job from participant job set               */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveParticipant(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
/* job.removeRetired -- remove job from retired job set                       */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveRetired(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
/* job::retire -- retire the job                                              */
/*----------------------------------------------------------------------------*/
void Step::vJobRetire(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;
  
  pJob->vJobState(JS_COMPLETED);
  
  vGrabTaskLock((Stz*)__func__);
  vScheduleDelete();

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
/* job.seedWord@ -- fetch job seed word                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedWordFetch(void)
{
  ENTRY();
 
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Word* pWord = pJob->pSeedWord();
  
  m_pDataStack->vPushPtr(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.seedWord! -- set job seed word                                         */
/*----------------------------------------------------------------------------*/
void Step::vJobSeedWordStore(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vGrabTaskLock((Stz*)__func__);

  RASSERT( _bIsValidJob(pJob->eGetClassNo()) );
  
  pJob->vJobState(JS_SIGNALLED);

  i8 i_Session = pJob->m_i_Session;
  
  Session* pSession = AM::pSessions()->pGetSession(i_Session);
  Sessions::vMarkSignalled(pSession);
  RASSERT( _bIsValidJob(pJob->eGetClassNo()) );
  

  
  vJob(nullptr);

  pSession->m_oMutex.vLock(this);
  pSession->m_oMutex.vSignal(this);
  pSession->m_oMutex.vUnlock(this);

  vDropTaskLock((Stz*)__func__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vScheduleCompletion();
  

  
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
/* job.phase@ -- fetch the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobStateFetch(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;

  m_pDataStack->vPushInt(pJob->eJobState());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase! -- store the job phase                                          */
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
/* job::tagSet@ -- push ptr to job TagSet                                      */
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





/*----------------------------------------------------------------------------*/
/* job[].get -- get task                                                      */
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

