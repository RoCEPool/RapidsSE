/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"
#include "ExtendedText.hpp"
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
  vAddStep("[job].addExited"_Stz,        &Step::vJobAddExited);
  vAddStep("[job].addLocal"_Stz,         &Step::vJobAddLocal);
  vAddStep("[job].addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("[job].addParticipant"_Stz,   &Step::vJobAddParticipant);
  vAddStep("[job].assertOrdinary"_Stz,   &Step::vJobAssertOrdinary);
  vAddStep("[job].assertOriginator"_Stz, &Step::vJobAssertOriginator);
  vAddStep("[job].assertParticipant"_Stz,&Step::vJobAssertParticipant);
  vAddStep("[job].assertReceived"_Stz,   &Step::vJobAssertReceived);
  vAddStep("[job].assertStructure"_Stz,  &Step::vJobAssertStructure);

  vAddStep("[job].cmdPrint"_Stz,         &Step::vJobCmdPrint);

  vAddStep("[job].disableAllocate"_Stz,  &Step::vJobDisableAllocate);
  vAddStep("[job].disableJobMetrics"_Stz, &Step::vJobDisableJobMetrics);
  vAddStep("[job].disableObjectTracker"_Stz,&Step::vJobDisableObjectTracker);
  vAddStep("[job].distId@"_Stz,          &Step::vJobDistIdFetch);
  vAddStep("[job].distIdGenerate"_Stz,   &Step::vJobDistIdGenerate);

  vAddStep("[job].enableAllocate"_Stz,   &Step::vJobEnableAllocate);
  vAddStep("[job].enableObjectTracker"_Stz,&Step::vJobEnableObjectTracker);
  vAddStep("[job].entryPoint@"_Stz,      &Step::vJobEntryPointFetch);
  vAddStep("[job].entryPoint!"_Stz,      &Step::vJobEntryPointStore);

  vAddStep("[job].error@"_Stz,           &Step::vJobErrorFetch);
  vAddStep("[job].error!"_Stz,           &Step::vJobErrorStore);

  vAddStep("[job].labelObjectTracker"_Stz,&Step::vJobLabelObjectTracker);
  vAddStep("[job].jobList@"_Stz,         &Step::vJobListFetch);

  vAddStep("[job].masterJobId"_Stz,      &Step::vJobMasterJobId);
  vAddStep("[job].mutex@"_Stz,           &Step::vJobMutexFetch);

  vAddStep("[job].name@"_Stz,            &Step::vJobNameFetch);
  vAddStep("[job].newMarkerChain"_Stz,   &Step::vJobNewMarkerChain);
  vAddStep("[job].new2PCOriginJobs"_Stz,  &Step::vJobNew2PCOriginJobs);
  vAddStep("[job].new2PCParticipantJobs"_Stz,  &Step::vJobNew2PCParticipantJobs);

  vAddStep("[job].originId@"_Stz,        &Step::vJobOriginIdFetch);
  vAddStep("[job].originNodeId@"_Stz,    &Step::vJobOriginNodeIdFetch);
  
  vAddStep("[job].parentId@"_Stz,        &Step::vJobParentIdFetch);
  vAddStep("[job].parentOriginNodeId@"_Stz, &Step::vJobParentOriginNodeIdFetch);
  vAddStep("[job].phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("[job].phase!"_Stz,           &Step::vJobPhaseStore);
  vAddStep("[job].preExecuteDist"_Stz,   &Step::vJobPreExecuteDist);
  vAddStep("[job].preExecuteLocal"_Stz,   &Step::vJobPreExecuteLocal);
  vAddStep("[job].printObjectTracker"_Stz,&Step::vJobPrintObjectTracker);
  vAddStep("[job].progress"_Stz,         &Step::vJobProgress);
  vAddStep("[job].progressReset"_Stz,    &Step::vJobProgressReset);
  vAddStep("[job].progressSeconds"_Stz,  &Step::vJobProgressSeconds);

  vAddStep("[job].rejectJobError!"_Stz,  &Step::vJobRejectJobErrorStore);
  vAddStep("[job].rejectJob@"_Stz,       &Step::vJobRejectJobFetch);
  vAddStep("[job].rejectJobSetup"_Stz,   &Step::vJobRejectJobSetup);
  vAddStep("[job].removeLocal"_Stz,      &Step::vJobRemoveLocal);
  vAddStep("[job].removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("[job].removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("[job].removeRetired"_Stz,    &Step::vJobRemoveRetired);
  vAddStep("[job].resetObjectTracker"_Stz,&Step::vJobResetObjectTracker);
  vAddStep("[job].responseOp@"_Stz,      &Step::vJobResponseOpFetch);
  vAddStep("[job].responseOp!"_Stz,      &Step::vJobResponseOpStore);

  vAddStep("[job].setInxPtr"_Stz,        &Step::vJobSetInxPtr);
  vAddStep("[job].setOriginId"_Stz,      &Step::vJobSetOriginId);
  vAddStep("[job].signalParent"_Stz,     &Step::vJobSignalParent);

  vAddStep("[job].tagSet@"_Stz,          &Step::vJobTagSetFetch);
  vAddStep("[job].task@"_Stz,            &Step::vJobTaskFetch);
  vAddStep("[job].trace"_Stz,            &Step::vJobTrace);
  vAddStep("[job].triggerBegun"_Stz,     &Step::vJobTriggerBegun);
  vAddStep("[job].triggerEnded"_Stz,     &Step::vJobTriggerEnded);
  vAddStep("[job].triggerEnd"_Stz,       &Step::vJobTriggerEnd);
  vAddStep("[job].triggerCancelled"_Stz, &Step::vJobTriggerCancelled);
  vAddStep("[job].triggerExecute"_Stz,   &Step::vJobTriggerExecute);
  vAddStep("[job].triggerExecuted"_Stz,  &Step::vJobTriggerExecuted);
  vAddStep("[job].tscBeginning"_Stz,     &Step::vJobTscBeginning);
  vAddStep("[job].tscBegun"_Stz,         &Step::vJobTscBegun);
  vAddStep("[job].tscExecuting"_Stz,     &Step::vJobTscExecuting);
  vAddStep("[job].tscExecuted"_Stz,      &Step::vJobTscExecuted);
  vAddStep("[job].tscEnding"_Stz,        &Step::vJobTscEnding);
  vAddStep("[job].tscEnded"_Stz,         &Step::vJobTscEnded);
  vAddStep("[job].tscCanceling"_Stz,     &Step::vJobTscCanceling);
  vAddStep("[job].tscCanceled"_Stz,      &Step::vJobTscCanceled);
  vAddStep("[job].tscParticipant"_Stz,   &Step::vJobTscParticipant);

  vAddStep("[job].updateParent"_Stz,     &Step::vJobUpdateParent);

  vAddStep("job::id@"_Stz,               &Step::vJobOtherIdFetch);
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

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  AM::vJob_AddDemon(pJob);
  
  //vTrace("%p ID %lld added to originator\n"_Stz, pJob, pJob->iJobId());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].addExited -- add job to exited job set                               */
/*----------------------------------------------------------------------------*/
void Step::vJobAddExited(void)
{
  ENTRY();

  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

//  AM::pParticipatingJobs()->vGrabLock();
//  AM::pParticipatingJobs()->vAddJob(pJob);
//  AM::pParticipatingJobs()->vDropLock();

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  RASSERT(pJob->pTask());

//  vTrace("Addingd Job %p ID %lld to retired list\n"_Stz,
//         pJob,
//         pJob->iJobId() & 0xFFFFFFFF
//         );

  AM::vJob_AddExited(pJob);


  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].addLocal -- add job to local job set                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobAddLocal(void)
{
  ENTRY();
  
  //Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  AM::vJob_AddLocal(pJob);
  
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

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  if (AM::pOriginatingJobs()->bLatch()) {
    vTrace("Unable to append job to OL as list is latched due to NODELOSS\n"_Stz);
    m_pDataStack->vPushInt(false);
  }
  else {
    pJob->vOriginatorJob(true);
    AM::vJob_AddOriginating(pJob);
    m_pDataStack->vPushInt(true);
  }


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

  //                                                                        HACK
  //m_pJob->vDistId(m_pJob->tJobId());

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  if (AM::pOriginatingJobs()->bLatch()) {
    vTrace("Unable to append job to OL as list is latched due to NODELOSS\n"_Stz);
    m_pDataStack->vPushInt(false);
  }
  else {
    AM::vJob_AddParticipating(pJob);
    m_pDataStack->vPushInt(true);
  }

  //vTrace("%p ID %lld added to participant\n"_Stz, pJob, pJob->tJobId().iAsI8());
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertBegin -- assert job is not in originator nor participant set   */
/*----------------------------------------------------------------------------*/
/*
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
    
    Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tDistId());
    if (! pJobFound)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "[job].assertBegin: "
             "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
    
    pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tDistId());
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
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tOriginId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tDistId());
  if (pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertBegin: "
           "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  EXIT();
}
*/

/*----------------------------------------------------------------------------*/
/* [job].assertOrdinary -- assert job is not in originator nor participant set*/
/*----------------------------------------------------------------------------*/
void Step::vJobAssertOrdinary(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  if (JL_NONE != pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOrdinary: "
           "Job::m_eJobList is not JL_none [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  if ( ! pJob->tOriginId().bIsUnknown() ) {
    Job *pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tOriginId());
    if (pJobFound)
      vThrow(FK_FATAL, __func__, __FILE__, __LINE__,
             "[job].assertOrdinary: "
             "Job found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()], AM::pGrid()->iThisNodeId());
  }

  if ( ! pJob->tDistId().bIsUnknown() ) {
    Job *pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tDistId());
    if (pJobFound)
      vThrow(FK_FATAL, __func__, __FILE__, __LINE__,
             "[job].assertOrdinary: "
             "Job found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
             aCN[pJob->eGetClassNo()], AM::pGrid()->iThisNodeId());
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertOriginator -- assert job is in originator job set              */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertOriginator(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  if (JL_ORIGINATING != pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_eJobList is not JL_ORIGINATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  if (pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job::m_bWasReceived is true [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  Job* pJobFound = AM::pOriginatingJobs()->pFindJob(pJob->tOriginId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertOriginator: "
           "Job not found in OriginatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertParticipant -- assert job is in participant job set            */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertParticipant(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  if (JL_PARTICIPATING != pJob->eJobList())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job::m_eJobList is not JL_PARTICIPATING [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(pJob->tDistId());
  if (! pJobFound)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertParticipant: "
           "Job not found in ParticipatingJobs [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertReceived -- assert job was received                            */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertReceived(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);

  if (! pJob->bWasReceived())
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "[job].assertReceived: "
           "Job::m_bWasReceived is false [Job %s on node %lld]\n"_Stz,
           aCN[pJob->eGetClassNo()],AM::pGrid()->iThisNodeId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].assertStructure -- assert structural correctness                     */
/*----------------------------------------------------------------------------*/
void Step::vJobAssertStructure(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(*m_pJob == pJob);
  
  RASSERT(pJob);
  RASSERT(pJob->pTask() == this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [Job.Cmd].print -- print cmd[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vJobCmdPrint(void)
{
  ENTRY();

  if (m_pJob->szCmdText())
    vTrace("%s\n"_Stz,(m_pJob->szCmdText()));
  else
    vTrace("<NO CMD TEXT>\n"_Stz);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [job].disableAllocate -- disable memory allocation for the job             */
/*----------------------------------------------------------------------------*/
void Step::vJobDisableAllocate(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  pJob->vSetAllocate(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].disableJobMetrics -- disable job metrics for the job                 */
/*----------------------------------------------------------------------------*/
void Step::vJobDisableJobMetrics(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  pJob->vJobMetrics(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].disableObjectTracker -- disable object tracking for this job         */
/*----------------------------------------------------------------------------*/
void Step::vJobDisableObjectTracker(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  pJob->vDisableObjectTracker();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].distId@ -- fetch the distributed id of the job                       */
/*----------------------------------------------------------------------------*/
void Step::vJobDistIdFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  m_pDataStack->vPushInt(pJob->tDistId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].distIdGenerate -- generate a distributed id for the job              */
/*----------------------------------------------------------------------------*/
void Step::vJobDistIdGenerate(void)
{
  ENTRY();
    
  //m_pJob->vDistId(m_pJob->tParentId());

  Job* pJob = pOuterJobOfTask();
  pJob->vDistIdGenerate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].enableAllocate -- enable memory allocation for the job               */
/*----------------------------------------------------------------------------*/
void Step::vJobEnableAllocate(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  pJob->vSetAllocate(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].enableObjectTracker -- enable object tracking for this job           */
/*----------------------------------------------------------------------------*/
void Step::vJobEnableObjectTracker(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  pJob->vEnableObjectTracker();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].entryPoint@ -- fetch the job sen entry code                          */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryPointFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushStz(pJob->szEntryPoint());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].entryPoint! -- store the job entry code                              */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryPointStore(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

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

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushInt(pJob->eJobError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].error! -- store the job error                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorStore(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  i8 iJobError = m_pDataStack->iPopAsInt(__func__);
  
  //pJob->m_xNode[iNode].m_eErrorCode = (eEC)iJobError;
  pJob->vJobError((eEC)iJobError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].get -- get task                                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobGet(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  
  m_pDataStack->vPushPtr(pJob);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].labelObjectTracker -- Give this object tracker a label               */
/*----------------------------------------------------------------------------*/
void Step::vJobLabelObjectTracker(void)
{
  ENTRY();

  s256 sEntry;
  m_pDataStack->vPopStr(sEntry,__func__);

  Job* pJob = * m_pJob;
  m_pJob->vLabelObjectTracker(sEntry);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].jobList@ -- fetch job list                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobListFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushInt(pJob->eJobList());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].masterJobId -- get the master job id of this (participant/local) job */
/*----------------------------------------------------------------------------*/
void Step::vJobMasterJobId(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  JobId tJobId = pJob->tOriginId();
  if ( ! tJobId.bIsUnknown() ) {
    m_pDataStack->vPushInt(tJobId.iAsI8());
  }
  else {
    tJobId = pJob->tJobId();
    RASSERT(! tJobId.bIsUnknown() );
    m_pDataStack->vPushInt(tJobId.iAsI8());
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].mutex@ -- fetch the job mutex state                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobMutexFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushInt(pJob->eMutexSide());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].name@ -- get name of job                                             */
/*----------------------------------------------------------------------------*/
void Step::vJobNameFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  s32 szName = pJob->szJobNm();
  m_pDataStack->vPushStr(szName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].newMarkerChain -- new the job marker chain                           */
/*----------------------------------------------------------------------------*/
void Step::vJobNewMarkerChain(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  pJob->vNewMarkerChain();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].new2PCOriginJobs -- new the 2pc jobs for this originating job        */
/*----------------------------------------------------------------------------*/
void Step::vJobNew2PCOriginJobs(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  pJob->vEndJob(Api::pCreateJob2PC(pJob, "end"_Stz));
  pJob->vExecuteJob(Api::pCreateJob2PC(pJob, "execute"_Stz));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].new2PCParticipantJobs -- new the 2pc response jobs for this job      */
/*----------------------------------------------------------------------------*/
void Step::vJobNew2PCParticipantJobs(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  pJob->vBegunJob(Api::pCreateJob2PC(pJob, "begun"_Stz));
  pJob->vEndedJob(Api::pCreateJob2PC(pJob, "ended"_Stz));
  pJob->vCancelledJob(Api::pCreateJob2PC(pJob, "Cancelled"_Stz));
  pJob->vExecutedJob(Api::pCreateJob2PC(pJob, "executed"_Stz));

  // set up the rejectJob
  pJob->pRejectJob()->vOriginId(Job::tOriginId(pJob));
  pJob->pRejectJob()->vDistId(Job::tDistId(pJob));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].originId@ -- fetch the origin id of the job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginIdFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  m_pDataStack->vPushInt(pJob->tOriginId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].originNodeId@ -- fetch the originator node no from this job          */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginNodeIdFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  JobId tOriginId = pJob->tOriginId();
  
  i8 iOriginNodeId = (i8)tOriginId.uNodeId();
  
  m_pDataStack->vPushInt(iOriginNodeId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].parentId@ -- fetch the parent job id of the job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobParentIdFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  m_pDataStack->vPushInt(pJob->tParentId().iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].parentOriginNodeId@ -- fetch origin node of of this job's parent job */
/*----------------------------------------------------------------------------*/
void Step::vJobParentOriginNodeIdFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  JobId tParentJobId = pJob->tParentId();
  
  i8 iNodeId = (i8)tParentJobId.uNodeId();
  
  m_pDataStack->vPushInt(iNodeId);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [job].phase@ -- fetch the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushInt(pJob->eJobPhase());
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [job].preExecuteDist -- test if distributed job can be run                 */
/*----------------------------------------------------------------------------*/
void Step::vJobPreExecuteDist(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  eEC eError = AM::JOB::ePreExecuteDist(pJob);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].preExecuteLocal -- test if local job can be run                      */
/*----------------------------------------------------------------------------*/
void Step::vJobPreExecuteLocal(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  eEC eError = AM::JOB::ePreExecuteLocal(pJob);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].phase! -- store the job phase                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseStore(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  i8 iJobPhase = m_pDataStack->iPopAsInt(__func__);
  
  pJob->vJobPhase((eJP)iJobPhase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].printObjectTracker -- object tracker print                           */
/*----------------------------------------------------------------------------*/
void Step::vJobPrintObjectTracker(void)
{
  ENTRY();

  Job* pJob = * m_pJob;
  m_pJob->vObjectTrackerDump();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].progress -- progress print                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobProgress(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  Timer* pProgress = pJob->pProgressTimer();

  pProgress->vUpdate();
  
  s4K sPrint;
  m_pDataStack->vPopStr(sPrint,__func__);
  
  vTrace("%s %lldus\n"_Stz
         ,sPrint.szStz()
         ,pProgress->iMicroseconds());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].progressReset -- progress reset                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobProgressReset(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
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

  Job* pJob = pOuterJobOfTask();
    
  Timer* pProgress = pJob->pProgressTimer();

  pProgress->vUpdate();
  
  i8 iSeconds = pProgress->iSeconds();

  m_pDataStack->vPushInt(iSeconds);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].rejectJobError! -- store reject job error                            */
/*----------------------------------------------------------------------------*/
void Step::vJobRejectJobErrorStore(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
 eEC eRejectError = (eEC) m_pDataStack->iPopInt(__func__);

  pJob->pRejectJob()->vError(eRejectError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].rejectJob@ -- fetch reject job                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobRejectJobFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushPtr(pJob->pRejectJob());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].rejectJobSetup -- set up the rejectJob                               */
/*----------------------------------------------------------------------------*/
void Step::vJobRejectJobSetup(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  // set up the rejectJob
  pJob->pRejectJob()->vOriginId(Job::tOriginId(pJob));
  pJob->pRejectJob()->vDistId(Job::tDistId(pJob));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeLocal -- remove job from local job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveLocal(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  
  AM::vJob_RemoveLocal(pJob);
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeOriginator -- remove job from originator job set               */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveOriginator(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  
  AM::vJob_RemoveOriginating(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeParticipant -- remove job from participant job set             */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveParticipant(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  
  AM::vJob_RemoveParticipating(pJob);
     
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].removeRetired -- remove job from retired job set                     */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveRetired(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  
  AM::vJob_RemoveRetired(pJob);
     
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].resetObjectTracker -- reset object tracking for this job             */
/*----------------------------------------------------------------------------*/
void Step::vJobResetObjectTracker(void)
{
  ENTRY();

  m_pJob->vResetObjectTracker();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].responseOp@ -- fetch response op                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseOpFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushPtr(pJob->p_ResponseOp());

  EXIT();
}
  
/*----------------------------------------------------------------------------*/
/* [job].responseOp! -- store response op                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseOpStore(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  Op* pResponseOp = _pOp(m_pDataStack->pPopPtr(__func__));

  pJob->vResponseOp(pResponseOp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].setInxPtr -- set the intruction pointr from entryPoint               */
/*----------------------------------------------------------------------------*/
void Step::vJobSetInxPtr(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  vSetInxPtr(pJob->szEntryPoint());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].setOriginId -- set the origin id of this job                         */
/*----------------------------------------------------------------------------*/
void Step::vJobSetOriginId(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  
  JobId tJobId;

  tJobId = pJob->tJobId();
  pJob->vOriginId(tJobId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].signalParent -- signal parent job waiting on this job/session mutex  */
/*----------------------------------------------------------------------------*/
void Step::vJobSignalParent(void)
{
  ENTRY();

  // there will be a thread (of parent job) waiting on completion of the job
  // this could be a prompt thread

  Job* pJob = pOuterJobOfTask();

  eJL eJobList = pJob->eJobList();

  RASSERT(JL_NONE        == eJobList ||
          JL_ORIGINATING == eJobList ||
          JL_RETIRED     == eJobList ||
          JL_DEMON       == eJobList)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eEC eErrorCode = AM::JLIST::eSignalParent(pJob);

  if (eErrorCode == EC_NONE) {
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tagSet@ -- push ptr to job TagSet                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobTagSetFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushPtr(pJob->pJobTags());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].task@ -- fetch the task executing the job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobTaskFetch(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  m_pDataStack->vPushPtr(pJob->pTask());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].trace -- distributed job trace                                       */
/*----------------------------------------------------------------------------*/
void Step::vJobTrace(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  s256 sNote;
  m_pDataStack->vPopStr(sNote,__func__);
  s256 sAtPoint;
  m_pDataStack->vPopStr(sAtPoint,__func__);
  s256 sAtRoutine;
  m_pDataStack->vPopStr(sAtRoutine,__func__);
  s256 sAtCmd;
  m_pDataStack->vPopStr(sAtCmd,__func__);

  pJob->vJobTrace(sAtCmd,sAtRoutine,sAtPoint,sNote);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].triggerBegun -- trigger the begun 2pc job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobTriggerBegun(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  Job* pBegunJob = pJob->pBegunJob();
  pJob->vBegunJob(nullptr);
  Api::vTriggerChildJob(pJob, pBegunJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].triggerCancelled -- trigger the cancelled 2pc job                    */
/*----------------------------------------------------------------------------*/
void Step::vJobTriggerCancelled(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  Job* pCancelledJob = pJob->pCancelledJob();
  pJob->vCancelledJob(nullptr);
  Api::vTriggerChildJob(pJob, pCancelledJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].triggerEnd -- trigger the end 2pc job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobTriggerEnd(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  Job* pEndJob = pJob->pEndJob();
   pJob->vEndJob(nullptr);
  Api::vTriggerChildJob(pJob, pEndJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].triggerEnded -- trigger the ended 2pc job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobTriggerEnded(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  Job* pEndedJob = pJob->pEndedJob();
  pJob->vEndedJob(nullptr);
  Api::vTriggerChildJob(pJob, pEndedJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].triggerExecute -- trigger the execute 2pc job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobTriggerExecute(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  Job* pExecuteJob = pJob->pExecuteJob();
  pJob->vExecuteJob(nullptr);
  Api::vTriggerChildJob(pJob, pExecuteJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].triggerExecuted -- trigger the executed 2pc job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobTriggerExecuted(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();

  Job* pExecutedJob = pJob->pExecutedJob();
  pJob->vExecutedJob(nullptr);
  Api::vTriggerChildJob(pJob, pExecutedJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscBeginning -- set tsc for beginning                                */
/*----------------------------------------------------------------------------*/
void Step::vJobTscBeginning(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscBeginning(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscBegun -- set tsc for begun                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobTscBegun(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscBegun(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscExecuting -- set tsc for executing                                */
/*----------------------------------------------------------------------------*/
void Step::vJobTscExecuting(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscExecuting(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscExecuted -- set tsc for executed                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobTscExecuted(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscExecuted(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscEnding -- set tsc for ending                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobTscEnding(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscEnding(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscEnded -- set tsc for ended                                        */
/*----------------------------------------------------------------------------*/
void Step::vJobTscEnded(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscEnded(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscCanceling -- set tsc for canceling                                */
/*----------------------------------------------------------------------------*/
void Step::vJobTscCanceling(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscCanceling(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscCanceled -- set tsc for canceled                                  */
/*----------------------------------------------------------------------------*/
void Step::vJobTscCanceled(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscCanceled(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].tscParticipant -- set tsc for participant                            */
/*----------------------------------------------------------------------------*/
void Step::vJobTscParticipant(void)
{
  ENTRY();

  Job* pJob = pOuterJobOfTask();
  RASSERT(m_pJob == pJob);

  u4 uProcId;
  pJob->vTscParticipant(Hardware::iReadTsc(&uProcId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [job].updateParent -- update parent job on information from this job       */
/*----------------------------------------------------------------------------*/
void Step::vJobUpdateParent(void)
{
  ENTRY();

  // there will be a thread (of parent job) waiting on completion of the job
  // this could be a prompt thread

  eEC eErrorCode = EC_NONE;
  Job* pParentJob;
  Job* pJob = pOuterJobOfTask();

  if ( ! pJob->bUpdateParent() )
    goto L_Exit;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the timer of this job
  pJob->vResponseTimerUpdate();

  eErrorCode = AM::JLIST::eUpdateParent(pJob);

  L_Exit:

  if (eErrorCode == EC_NONE) {
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*-- Job Param ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job::id@ -- fetch the job id of other job (param)                          */
/*----------------------------------------------------------------------------*/
void Step::vJobOtherIdFetch(void)
{
  ENTRY();

  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  m_pDataStack->vPushInt(pJob->tJobId().iAsI8());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
