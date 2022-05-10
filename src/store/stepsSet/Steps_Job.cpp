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
#include "Stream.hpp"
#include "Op.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "Vertex.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "JobList.hpp"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Job(void)
{
  vAddStep("[job]"_Stz,                  &Step::vJobGet);

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
    
  vAddStep("[job].originNodeId@"_Stz,    &Step::vJobOriginNodeIdFetch);
  
  vAddStep("[job].parentId@"_Stz,        &Step::vJobParentIdFetch);
  vAddStep("[job].parentOriginNodeId@"_Stz, &Step::vJobParentOriginNodeIdFetch);
  vAddStep("[job].participantNodeId@"_Stz, &Step::vJobParticipantNodeIdFetch);
  vAddStep("[job].participantNodeId!"_Stz, &Step::vJobParticipantNodeIdStore);
  vAddStep("[job].phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("[job].phase!"_Stz,           &Step::vJobPhaseStore);
  vAddStep("[job].preExecuteTest"_Stz,   &Step::vJobPreExecuteTest);
  vAddStep("[job].progress"_Stz,         &Step::vJobProgress);
  vAddStep("[job].progressReset"_Stz,    &Step::vJobProgressReset);
  vAddStep("[job].progressSeconds"_Stz,  &Step::vJobProgressSeconds);

  vAddStep("[job].reapable@"_Stz,        &Step::vJobReapableFetch);
  vAddStep("[job].reapable!"_Stz,        &Step::vJobReapableStore);
  vAddStep("[job].removeLocal"_Stz,      &Step::vJobRemoveLocal);
  vAddStep("[job].removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("[job].removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("[job].removeRetired"_Stz,    &Step::vJobRemoveRetired);
    
  vAddStep("[job].signalWaiter"_Stz,     &Step::vJobSignalWaiter);
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
  
  vAddStep("[job].wasForked?"_Stz,       &Step::vJobWasForkedFetch);
  vAddStep("[job].wasForked!"_Stz,       &Step::vJobWasForkedStore);
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
  

  if ((i8)pJob->tJobId().uNodeId() == AM::pGrid()->iThisNodeId()) {

    if (! pJob->bWasReceived())
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertOriginator: "
             "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
    
    if (JL_NONE != pJob->eJobList())
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertBegin: "
             "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
    
    Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tJobId());
    if (! pJobFound)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertBegin: "
             "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
    
    pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tJobId());
    if (pJobFound)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertBegin: "
             "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  if (JL_NONE != pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
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
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(m_pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOrdinary: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  pJobFound = AM::pParticipatingJobs()->pFindJob(m_pJob->tJobId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOrdinary: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertOriginator -- assert job is in originator job set              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertOriginator(void)
{
  ENTRY();
    
  if (JL_ORIGINATING != m_pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_eJobList is not JL_ORIGINATING [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  if (m_pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_bWasReceived is true [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(m_pJob->tJobId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertParticipant -- assert job is in participant job set            */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertParticipant(void)
{
  ENTRY();
    
  if (JL_PARTICIPATING != m_pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job::m_eJobList is not JL_PARTICIPATING [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  if (! m_pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(m_pJob->tJobId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job not found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

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
           aCN[m_pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
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
/* [job].entryPoint@ -- fetch the job sen entry code                          */
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
/* [job].entryPoint! -- store the job entry code                              */
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
/* [job].originNodeId@ -- fetch the originator node no from this job          */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginNodeIdFetch(void)
{
  ENTRY();
  
  JobId tJobId = m_pJob->tJobId();
  
  i8 iNodeId = (i8)tJobId.uNodeId();
  
  m_pDataStack->vPushInt(iNodeId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].parentId@ -- fetch the parent job id of the job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobParentIdFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->tParentId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].parentOriginNodeId@ -- fetch origin node of of this job's parent job */
/*----------------------------------------------------------------------------*/
void Step::vJobParentOriginNodeIdFetch(void)
{
  ENTRY();
  
  JobId tParentJobId = m_pJob->tParentId();
  
  i8 iNodeId = (i8)tParentJobId.uNodeId();
  
  m_pDataStack->vPushInt(iNodeId);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job.participantNodeId@ -- fetch the participant node from the job          */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantNodeIdFetch(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  m_pDataStack->vPushInt(pJob->iParticipantNodeId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].participantNodeId! -- store the participant node in the job          */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantNodeIdStore(void)
{
  ENTRY();
  
  Job* pJob = * m_pJob;

  i8 iParticipantNodeId = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vParticipantNodeId(iParticipantNodeId);
  
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
/* [job].signalWaiter -- signal the actor waiting on this job/session mutex   */
/*----------------------------------------------------------------------------*/
void Step::vJobSignalWaiter(void)
{
  ENTRY();
  
  // there will be a thread waiting on completion of the job
  // this could be a prompt thread or a java thread via the connector
  
  //Job* pJob = MY::pJob();
  Job* pJob = * m_pJob;

  eJL eJobList = pJob->eJobList();
  RASSERT(JL_NONE == eJobList || JL_RETIRED == eJobList)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pJob->vMutexLock();
  pJob->vMutexSignal();
  pJob->vMutexUnlock();

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

/*----------------------------------------------------------------------------*/
/* [job].wasForked? -- fetch if the job was forked?                           */
/*----------------------------------------------------------------------------*/
void Step::vJobWasForkedFetch(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;

  m_pDataStack->vPushInt(pJob->bWasForked());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].phase! -- store if the job was forked                                */
/*----------------------------------------------------------------------------*/
void Step::vJobWasForkedStore(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;

  i8 iWasForked = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vWasForked((bool)iWasForked);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
