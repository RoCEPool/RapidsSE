/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Op.hpp"

#include "Job.hpp"
#include "File.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "Output.hpp"
#include "Api.hpp"

#include "TagConstants.h"

#include "SP_Gid.hpp"
#include "SP_Timer.hpp"

#include "SP_Pointer.hpp"

extern pthread_key_t giTLS_CurrentPoint;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Job,CN_JOB)
__aNewFunction(Job,CN_JOB)
__vDeleteFunction(Job,CN_JOB)
__aFooterFunction(Job)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId, TagSet* pCmdTagSet)
: Operand(CN_JOB)
, CRM<Job>(this)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
 
  m_tJobId.vNextJobId();
  m_tParentId = tParentId;
  m_tDistId   = tDistId;
  m_tOriginId = tOriginId;

  m_sJobNm   = szJobNm;

  m_pObjectTracker = nullptr;
  m_bPersistentTx = false;

  // Before the entire Job is created, check the cmd tag set to see if objectTracker
  // tag is given, to enable object tracking in this job.
  if(pCmdTagSet != nullptr && pCmdTagSet->bHasName(C_TAG_OBJTRACKER))
  {
    vEnableObjectTracker();
  }

  //m_sEntryPoint
  //m_oJobTags
  //m_sFolderPath
  
  m_eJobPhase  = JP_NONE;
  m_eError_job = EC_NONE;
  m_eChildError = EC_NONE;
  m_bOriginatorJob = false;
  m_bNodeLoss     = false;
  m_bUpdateParent = false;

  m_bJobMetrics   = true;

  m_pMetricsPipeline = nullptr;

  m_pExecuteJob   = nullptr;
  m_pEndJob       = nullptr;

  m_pBegunJob     = nullptr;
  m_pExecutedJob  = nullptr;
  m_pEndedJob     = nullptr;
  m_pCancelledJob = nullptr;
  m_pRejectJob    =  new RejectJob;

  /* 00 */ m_noActiveOps.vResetAtomic();
  /* 01 */ m_eJobList     = JL_NONE;
  /* 02 */ m_pJobMetrics  = nullptr;
  /* 03 */ m_pMutex       = new Mutex(this);
  /* 04 */ m_pOutput      = nullptr;
  /* 05 */ m_p_ResponseOp = nullptr;
  /* 06 */ m_oResponseTimer.vStart();
  /* 07 */ m_pTask        = nullptr;
  /* 08 */ m_bWasReceived = false;
  /* 09 */ m_pMarkerChain = nullptr;
  /* 10 */ m_bAllocate    = true;

    m_iSysTimeStart       = Hardware::uReadClock();

#ifdef WITH_OOM_FAULTINJECTION
  PsxThd::vTlsSet(giTLS_CurrentPoint,(void*)5);
#endif

  /* 11 */ m_pJobsSkipNode    = new(C_SKIPLIST_LEVELS,nullptr) SkipNode(this);
  /* 12 */ m_pGenericSkipNode = new(C_SKIPLIST_LEVELS,nullptr) SkipNode(this);

  m_iTscParticipant = 0;
  
  m_iTscBeginning   = 0;
  m_iTscBegun       = 0;
  m_iTscExecuting   = 0;
  m_iTscExecuted    = 0;
  m_iTscEnding      = 0;
  m_iTscEnded       = 0;
  m_iTscCanceling   = 0;
  m_iTscCanceled    = 0;

  m_iSysTimeEnd     = 0;

  // -- -- -- -- -- --
        
  _checkInitialised_(CN_JOB);
  
  AM::JOBS::vJobsActiveInc();
  
  m_oJobTags.vInsert(C_TAG_PATH,C_SE_PATH);

  AM::JLIST::vInsertJob(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  try {
    if (AM::JLIST::bFindJob(this))
      AM::JLIST::vRemoveJob(this);

    delete m_pJobsSkipNode;
    delete m_pGenericSkipNode;

    //  if (bHasTag(C_TAG_LIFE) || JK_GENERIC == m_eJobKind) {
    //
    //    SP_TagSet spTagSet;
    //    vPrint("Job Delete: %s\n"_Stz,spTagSet.sz(&m_oJobTags));
    //    //m_oJobTags.vPrint();
    //  }

    // -- -- -- -- -- --

    //! this assert may fail if a job completes before its references
    //! are completely returned
    // NOTE: TaskState can be TS_NONE for 2PC jobs that never got triggered eg CANCELLED
    while (nReferences() ||
           (m_pTask &&
             (TS_RETIRED != m_pTask->eState() &&
              TS_NONE != m_pTask->eState()))) {
      //vTrace("Napping ... job still has references???\n"_Stz);
      AM::vNapMsec(10, eGetClassNo());
    }

    //RASSERT(m_bReapable);

    if (m_pJobMetrics) {
      while (m_pJobMetrics) {
        JobMetrics *pJobMetricsNext = m_pJobMetrics->pNext();
        delete m_pJobMetrics;
        m_pJobMetrics = pJobMetricsNext;
      }

    }

    if (m_pMetricsPipeline) {
      delete m_pMetricsPipeline;
    }

    if (m_pTask) {
      m_pTask->vGrabTaskLock((Stz *) __func__);
      m_pTask->vState(TS_DELETING);
      m_pTask->vDropTaskLock((Stz *) __func__);
      delete m_pTask;
    }

    AM::JOBS::vJobsActiveDec();

    if (m_pMutex)
      vMutexDestroy();

    //RASSERT(m_pMutex);
    //vMutexDestroy();

    /*
    i8 iResult;

    iResult = pthread_mutex_destroy(&m_pMutex->m_xMutex);
    if ( iResult ) {
      vTrace("UNABLE TO DESTROY MUTEX OF Job: %s:%s\n"_Stz,
             m_sJobNm.azChar(),
             m_sEntryPoint.azChar());
    }
    RASSERT(! iResult);
    iResult = pthread_mutexattr_destroy(&m_pMutex->m_xMutexAttr);
    RASSERT(! iResult);

    iResult = pthread_cond_destroy(&m_pMutex->m_xCond);
    RASSERT(! iResult);
    iResult = pthread_condattr_destroy(&m_pMutex->m_xCondAttr);
    RASSERT(! iResult);

    m_pMutex->m_bAlreadyDestroyed = true;

    vMutexDestroy();
  */

    if (m_pExecuteJob)
      delete m_pExecuteJob;

    if (m_pEndJob)
      delete m_pEndJob;

    if (m_pBegunJob)
      delete m_pBegunJob;

    if (m_pExecutedJob)
      delete m_pExecutedJob;

    if (m_pEndedJob)
      delete m_pEndedJob;

    if (m_pCancelledJob)
      delete m_pCancelledJob;

    delete m_pRejectJob;

    if (m_pOutput)
      vOutputDestroy();

    if (m_pMarkerChain)
      delete m_pMarkerChain;

    if(bObjectTracker())
      delete m_pObjectTracker;
  }
  catch (...) {
    vTrace("EXCEPTION in ~JOB: %s %s %s %s %s %s\n"_Stz,
           m_sJobNm.azChar(),
           m_sEntryPoint.azChar(),
           aJP[m_eJobPhase],
           m_pTask ? (m_pTask->bIsLocked() ? "isLocked" : "notLocked" ): "unknown",
           aEC[m_eError_job],
           aEC[m_eError_job]);
    throw ;
  }
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get dist job id                                                            */
/*----------------------------------------------------------------------------*/
JobId Job::tDistId(Job* pJob)
{
  _static_

  return pJob->m_tDistId;
}

/*----------------------------------------------------------------------------*/
/* get the job name                                                           */
/*----------------------------------------------------------------------------*/
Stz* Job::szJobNm(Job* pJob)
{
  _static_

  return pJob->m_sJobNm;
}

/*----------------------------------------------------------------------------*/
/* get origin job id                                                          */
/*----------------------------------------------------------------------------*/
JobId Job::tOriginId(Job* pJob)
{
  _static_

  return pJob->m_tOriginId;
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Job::vPostRecv(void* aReceivedJob)
{
  _static_
  
  Job* pReceivedJob = _pJob(aReceivedJob);
  
  pReceivedJob->vPostRecv();
  
}

/*----------------------------------------------------------------------------*/
/* if the key is greater than object on the right, then move right            */
/*----------------------------------------------------------------------------*/
void Job::vPrintJobNode(SkipNode* pNode, int i_Level)
{
  _static_
  
  RASSERT(pNode);

  Job* pJob = _pJob(pNode->m_pObject);
  
  JobId tJobId = pJob->tJobId();
  
  vTrace("@%lld %p : %lld.%lld -> %p\n"_Stz
         ,i_Level
         ,pNode
         ,(i8)tJobId.uNodeId()
         ,(i8)tJobId.uJobNo()
         ,pNode->m_pRight[i_Level]
         );
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the number of active ops                                               */
/*----------------------------------------------------------------------------*/
i8 Job::nActiveOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  i8 nActive = * m_noActiveOps;
  
  return nActive;
}

/*----------------------------------------------------------------------------*/
/* decrement number of active op's                                            */
/*----------------------------------------------------------------------------*/
void Job::vActiveOpsDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  -- m_noActiveOps;
}

/*----------------------------------------------------------------------------*/
/* icrement number of active op's                                             */
/*----------------------------------------------------------------------------*/
void Job::vActiveOpsInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  ++ m_noActiveOps;
}

/*----------------------------------------------------------------------------*/
/* is memory allocation allowed for this job?                                 */
/*----------------------------------------------------------------------------*/
bool Job::bAllocateMemory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_bAllocate;
}

/*----------------------------------------------------------------------------*/
/* get 2pc begunJob                                                           */
/*----------------------------------------------------------------------------*/
Job* Job::pBegunJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pBegunJob;
}

/*----------------------------------------------------------------------------*/
/* set 2pc begunJob                                                           */
/*----------------------------------------------------------------------------*/
void Job::vBegunJob(Job* pBegunJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pBegunJob = pBegunJob;
}

/*----------------------------------------------------------------------------*/
/* get 2pc cancelledJob                                                       */
/*----------------------------------------------------------------------------*/
Job* Job::pCancelledJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pCancelledJob;
}

/*----------------------------------------------------------------------------*/
/* set 2pc cancelledJob                                                       */
/*----------------------------------------------------------------------------*/
void Job::vCancelledJob(Job* pCancelledJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pCancelledJob = pCancelledJob;
}

/*----------------------------------------------------------------------------*/
/* get child job error                                                        */
/*----------------------------------------------------------------------------*/
eEC Job::eChildError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_eChildError;
}

/*----------------------------------------------------------------------------*/
/* set child job error                                                        */
/*----------------------------------------------------------------------------*/
void Job::vChildError(eEC eErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_eChildError = eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
Stz* Job::szCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  Stz* szCmd = m_oJobTags.szValue(C_TAG_CMD);
  
  return szCmd;
}

/*----------------------------------------------------------------------------*/
/* set command                                                                */
/*----------------------------------------------------------------------------*/
void Job::vCmd(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_oJobTags.vReplace(C_TAG_CMD,szCmd);
}

/*----------------------------------------------------------------------------*/
/* disable object tracker                                                     */
/*----------------------------------------------------------------------------*/
void Job::vDisableObjectTracker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  if(bObjectTracker()) {
    delete m_pObjectTracker;
  }
  m_pObjectTracker = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get distributed job id                                                     */
/*----------------------------------------------------------------------------*/
JobId Job::tDistId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_tDistId;
}

/*----------------------------------------------------------------------------*/
/* generate distributed job id                                                */
/*----------------------------------------------------------------------------*/
void Job::vDistIdGenerate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tDistId.vNextJobId();
}

/*----------------------------------------------------------------------------*/
/* get node where distributed job was forked                                  */
/*----------------------------------------------------------------------------*/
i8 Job::iDistNodeId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iDistNodeId = m_tDistId.uNodeId();

  return iDistNodeId;
}

/*----------------------------------------------------------------------------*/
/* drain the output queue                                                     */
/*----------------------------------------------------------------------------*/
void Job::vDrainOutput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT( ! m_p_ResponseOp );

  Job* pParentJob = this->pParentJob();

  if (pParentJob) {
    pParentJob->vDrainOutput();
  }
  else {
    RASSERT(m_pOutput);
    m_pOutput->vDrainBufferQueue();
  }
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Job::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* sen . print                                                                */
/*----------------------------------------------------------------------------*/
void Job::vDotPrint(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  SP_Timer spTimer;
  
  m_oResponseTimer.vUpdate();
  
  vTrace("[%s: job %s %s] "
         "%s"
         "\n"_Stz
         
         ,AM::pGrid()->pThisNode()->szNodeNm()
         ,m_sJobNm.szStz()
         ,spTimer.sz(&m_oResponseTimer)
         
         ,szText
         
         ,C_VARGS_END
         );
}

/*----------------------------------------------------------------------------*/
/* enable object tracker                                                      */
/*----------------------------------------------------------------------------*/
void Job::vEnableObjectTracker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if(!bObjectTracker()) {
    m_pObjectTracker = new ObjectTracker();
  }

}

/*----------------------------------------------------------------------------*/
/* enable persistence for this job                                            */
/*----------------------------------------------------------------------------*/
void Job::vEnablePersistence(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_bPersistentTx = true;

}

/*----------------------------------------------------------------------------*/
/* get 2pc end job                                                            */
/*----------------------------------------------------------------------------*/
Job* Job::pEndJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pEndJob;
}

/*----------------------------------------------------------------------------*/
/* set 2pc end job                                                            */
/*----------------------------------------------------------------------------*/
void Job::vEndJob(Job* pEndJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pEndJob = pEndJob;
}

/*----------------------------------------------------------------------------*/
/* get 2pc ended job                                                          */
/*----------------------------------------------------------------------------*/
Job* Job::pEndedJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pEndedJob;
}

/*----------------------------------------------------------------------------*/
/* set 2pc ended job                                                          */
/*----------------------------------------------------------------------------*/
void Job::vEndedJob(Job* pEndedJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pEndedJob = pEndedJob;
}

/*----------------------------------------------------------------------------*/
/* get the sen entry code                                                     */
/*----------------------------------------------------------------------------*/
Stz* Job::szEntryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sEntryPoint;
}

/*----------------------------------------------------------------------------*/
/* set the sen entry code                                                     */
/*----------------------------------------------------------------------------*/
void Job::vEntryPoint(Stz* szEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sEntryPoint = szEntry;
}

/*----------------------------------------------------------------------------*/
/* get 2pc executeJob                                                         */
/*----------------------------------------------------------------------------*/
Job* Job::pExecuteJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pExecuteJob;
}

/*----------------------------------------------------------------------------*/
/* set 2pc executJob                                                          */
/*----------------------------------------------------------------------------*/
void Job::vExecuteJob(Job* pExecuteJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pExecuteJob = pExecuteJob;
}

/*----------------------------------------------------------------------------*/
/* get 2pc executedJob                                                        */
/*----------------------------------------------------------------------------*/
Job* Job::pExecutedJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pExecutedJob;
}

/*----------------------------------------------------------------------------*/
/* set 2pc executedJob                                                        */
/*----------------------------------------------------------------------------*/
void Job::vExecutedJob(Job* pExecutedJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pExecutedJob = pExecutedJob;
}

/*----------------------------------------------------------------------------*/
/* get folder path                                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::szFolderPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sFolderPath;
}

/*----------------------------------------------------------------------------*/
/* set folder path                                                            */
/*----------------------------------------------------------------------------*/
void Job::vFolderPath(Stz* szFolderPath)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sFolderPath = szFolderPath;
}

/*----------------------------------------------------------------------------*/
/* free given marker                                                          */
/*----------------------------------------------------------------------------*/
void Job::vFreeMarker(Marker* pMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
//  if (m_pMarkerChain)
//    m_pMarkerChain->vPush(pMarker);
//  else
    delete pMarker;
}

/*----------------------------------------------------------------------------*/
/* get the generic skip node                                                  */
/*----------------------------------------------------------------------------*/
SkipNode* Job::pGenericSkipNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pGenericSkipNode;
}

/*----------------------------------------------------------------------------*/
/* update timer from the child job                                            */
/*----------------------------------------------------------------------------*/
void Job::vGetTimerFromChild(Job* pChildJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_oResponseTimer = pChildJob->m_oResponseTimer;

  m_iTscBeginning  = pChildJob->m_iTscBeginning;
  m_iTscBegun      = pChildJob->m_iTscBegun;
  m_iTscExecuting  = pChildJob->m_iTscExecuting;
  m_iTscExecuted   = pChildJob->m_iTscExecuted;
  m_iTscEnding     = pChildJob->m_iTscEnding;
  m_iTscEnded      = pChildJob->m_iTscEnded;
  m_iTscCanceling  = pChildJob->m_iTscCanceling;
  m_iTscCanceled   = pChildJob->m_iTscCanceled;

  m_iSysTimeStart  = pChildJob->m_iSysTimeStart;
  m_iSysTimeEnd    = pChildJob->m_iSysTimeEnd;
}

/*----------------------------------------------------------------------------*/
/* get sys/time.h start time                                                  */
/*----------------------------------------------------------------------------*/
i8 Job::iGetSysTimerStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  return m_iSysTimeStart;
}

/*----------------------------------------------------------------------------*/
/* get sys/time.h end time                                                    */
/*----------------------------------------------------------------------------*/
i8 Job::iGetSysTimerEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  return m_iSysTimeEnd;
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Job::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* test if this job has given ename tag                                       */
/*----------------------------------------------------------------------------*/
bool Job::bHasTag(Stz* szTagNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_oJobTags.bHasName(szTagNm))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is job in error?                                                           */
/*----------------------------------------------------------------------------*/
bool Job::bIsInError(void)
{
  return m_eError_job == EC_NONE ? false : true;
}

/*----------------------------------------------------------------------------*/
/* are we on the origin?                                                      */
/*----------------------------------------------------------------------------*/
bool Job::bIsOnOrigin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iOrigin = iOriginNodeId();
  i8 iParticipant = iDistNodeId();
 
  if (iParticipant == iOrigin)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this an originator job?                                                 */
/*----------------------------------------------------------------------------*/
bool Job::bIsOriginator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (JL_ORIGINATING == m_eJobList)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this an originator job?                                                 */
/*----------------------------------------------------------------------------*/
bool Job::bOriginatorJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_bOriginatorJob;
}

/*----------------------------------------------------------------------------*/
/* set originator job                                                         */
/*----------------------------------------------------------------------------*/
void Job::vOriginatorJob(bool bOriginatorJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_bOriginatorJob = bOriginatorJob;
}

/*----------------------------------------------------------------------------*/
/* is this an participant job?                                                 */
/*----------------------------------------------------------------------------*/
bool Job::bIsParticipant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (JL_PARTICIPATING == m_eJobList)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a pipeline job?                                                    */
/*----------------------------------------------------------------------------*/
bool Job::bIsPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (m_sJobNm.bSame("pipeline"_Stz) ||
      m_sJobNm.bSame("pipelineLocal"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a local pipeline job?                                              */
/*----------------------------------------------------------------------------*/
bool Job::bIsPipelineLocal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (m_sJobNm.bSame("pipelineLocal"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get job error                                                              */
/*----------------------------------------------------------------------------*/
eEC Job::eJobError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eError_job;
}

/*----------------------------------------------------------------------------*/
/* set job error                                                              */
/*----------------------------------------------------------------------------*/
void Job::vJobError(eEC eErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_eError_job = eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* get the job id                                                             */
/*----------------------------------------------------------------------------*/
JobId Job::tJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_tJobId;
}

/*----------------------------------------------------------------------------*/
/* add a label to the object tracker                                          */
/*----------------------------------------------------------------------------*/
void Job::vLabelObjectTracker(Stz *szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if(bObjectTracker())
  {
    m_pObjectTracker->vAddLabel(szLabel);
  }
}

/*----------------------------------------------------------------------------*/
/* get the job list                                                           */
/*----------------------------------------------------------------------------*/
eJL Job::eJobList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobList;
}

/*----------------------------------------------------------------------------*/
/* set the job list                                                           */
/*----------------------------------------------------------------------------*/
void Job::vJobList(eJL eJobList)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eJobList = eJobList;
}

/*----------------------------------------------------------------------------*/
/* set jobMetrics enabled/disabled                                            */
/*----------------------------------------------------------------------------*/
void Job::vJobMetrics(bool bJobMetrics)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_bJobMetrics = bJobMetrics;
}

/*----------------------------------------------------------------------------*/
/* get jobMetrics enabled/disabled                                            */
/*----------------------------------------------------------------------------*/
bool Job::bJobMetrics(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_bJobMetrics;
}

/*----------------------------------------------------------------------------*/
/* add job metrics to chain                                                   */
/*----------------------------------------------------------------------------*/
void Job::vJobMetricsAdd(JobMetrics *pJobMetrics)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  pJobMetrics->vNext(m_pJobMetrics);
  
  m_pJobMetrics = pJobMetrics;
}

/*----------------------------------------------------------------------------*/
/* get job metrics chain                                                      */
/*----------------------------------------------------------------------------*/
JobMetrics* Job::pJobMetricsChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_pJobMetrics;
}

/*----------------------------------------------------------------------------*/
/* get the job name                                                           */
/*----------------------------------------------------------------------------*/
Stz* Job::szJobNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sJobNm;
}

/*----------------------------------------------------------------------------*/
/* set the job name                                                           */
/*----------------------------------------------------------------------------*/
void Job::vJobNm(Stz* szJobNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sJobNm = szJobNm;
}

/*----------------------------------------------------------------------------*/
/* get the job phase                                                          */
/*----------------------------------------------------------------------------*/
eJP Job::eJobPhase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobPhase;
}

/*----------------------------------------------------------------------------*/
/* set the job phase                                                          */
/*----------------------------------------------------------------------------*/
void Job::vJobPhase(eJP eJobPhase)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eJobPhase = eJobPhase;
}

/*----------------------------------------------------------------------------*/
/* get the jobs skip node                                                     */
/*----------------------------------------------------------------------------*/
SkipNode* Job::pJobsSkipNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pJobsSkipNode;
}

/*----------------------------------------------------------------------------*/
/* get ename set                                                              */
/*----------------------------------------------------------------------------*/
TagSet* Job::pJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return & m_oJobTags;
}

/*----------------------------------------------------------------------------*/
/* trace job                                                                  */
/*----------------------------------------------------------------------------*/
void Job::vJobTrace(Stz* szAtCmd, Stz* szAtRoutine,
                    Stz* szAtPoint, Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  s4K sLine;
  
  sLine.vSprint("vJobTrace - %10s : %-14s @ %-8s # %-24s - "_Stz,
                szAtCmd,
                szAtRoutine,
                szAtPoint,
                szNote
                );
  
  if (m_tJobId.bIsUnknown())
    sLine.vCatSprint("%-11s"_Stz,"J:unknown "_Stz);
  else
    sLine.vCatSprint("J%04lld.%04lld "_Stz,
                     m_tJobId.uNodeId(),
                     m_tJobId.uJobNo()
                    );

  if (m_tParentId.bIsUnknown())
    sLine.vCatSprint("%-11s"_Stz,"P:unknown "_Stz);
  else
    sLine.vCatSprint("P%04lld.%04lld "_Stz,
                     m_tParentId.uNodeId(),
                     m_tParentId.uJobNo()
                    );

  sLine.vCatSprint(":: "_Stz);
  
  if (m_tOriginId.bIsUnknown())
    sLine.vCatSprint("%-11s"_Stz,"O:unknown "_Stz);
  else
    sLine.vCatSprint("O%04lld.%04lld "_Stz,
                     m_tOriginId.uNodeId(),
                     m_tOriginId.uJobNo()
                    );

  if (m_tDistId.bIsUnknown())
    sLine.vCatSprint("%-11s"_Stz,"D:unknown "_Stz);
  else
    sLine.vCatSprint("D%04lld.%04lld "_Stz,
                     m_tDistId.uNodeId(),
                     m_tDistId.uJobNo()
                    );

  sLine.vCatSprint("\n"_Stz);
  
  vTrace("%s"_Stz,sLine.szStz());
}

/*----------------------------------------------------------------------------*/
/* get last job id                                                            */
/*----------------------------------------------------------------------------*/
JobId Job::tLastJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  TagSet* pJobTags = this->pJobTags();

  if (! pJobTags->bHasName(C_TAG_LASTJOBID))
    return C_UNKNOWN;
  
  JobId tLastJobId = pJobTags->iValue(C_TAG_LASTJOBID);
  
  return tLastJobId;
}

/*----------------------------------------------------------------------------*/
/* set last job id                                                            */
/*----------------------------------------------------------------------------*/
void Job::vLastJobId(JobId tLastJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  TagSet* pJobTags = this->pJobTags();

  pJobTags->vReplace(C_TAG_LASTJOBID,tLastJobId.iAsI8());
}

/*----------------------------------------------------------------------------*/
/* get the mutex state                                                        */
/*----------------------------------------------------------------------------*/
eMS Job::eMutexSide(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  eMS eMutex = m_pMutex->eMutex();

  return eMutex;
}

/*----------------------------------------------------------------------------*/
/* create a new mutex for this job                                            */
/*----------------------------------------------------------------------------*/
void Job::vMutexCreate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(! m_pMutex);
  
  m_pMutex = new Mutex(this);
}

/*----------------------------------------------------------------------------*/
/* destroy this job's mutex                                                   */
/*----------------------------------------------------------------------------*/
void Job::vMutexDestroy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(m_pMutex);
  
  delete m_pMutex;
  m_pMutex = nullptr;
}

/*----------------------------------------------------------------------------*/
/* lock this job's mutex                                                      */
/*----------------------------------------------------------------------------*/
void Job::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_pMutex->vLock(this);
}

/*----------------------------------------------------------------------------*/
/* signal this job's mutex                                                    */
/*----------------------------------------------------------------------------*/
void Job::vMutexSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_pMutex->vLock(this);
  m_pMutex->vSignalAndUnlock(this);
}

/*----------------------------------------------------------------------------*/
/* unlock this job's mutex                                                    */
/*----------------------------------------------------------------------------*/
void Job::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_pMutex->vUnlock(this);
}

/*----------------------------------------------------------------------------*/
/* wait on this job's mutex                                                   */
/*----------------------------------------------------------------------------*/
void Job::vMutexWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
      
  //m_pMutex->vWaitForSignalLoop(this);
  m_pMutex->vPauseForSignalLoop(this,100000);
}

/*----------------------------------------------------------------------------*/
/* new a marker from the chain                                                */
/*----------------------------------------------------------------------------*/
Marker* Job::pNewMarker(eOM eOpMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_);
      
  Marker* pMarker = m_pMarkerChain->pPop();
  
  if (pMarker)
    pMarker->vOpMarker(eOpMarker);
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* new the marker chain                                                       */
/*----------------------------------------------------------------------------*/
void Job::vNewMarkerChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
      
  m_pMarkerChain = new MarkerChain;
}

/*----------------------------------------------------------------------------*/
/* get nodeLoss                                                               */
/*----------------------------------------------------------------------------*/
bool Job::bNodeLoss(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_bNodeLoss;
}

/*----------------------------------------------------------------------------*/
/* set nodeLoss                                                               */
/*----------------------------------------------------------------------------*/
void Job::vNodeLoss(bool bNodeLoss)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_bNodeLoss = bNodeLoss;
}

/*----------------------------------------------------------------------------*/
/* check if ObjectTracker is allocated                                        */
/*----------------------------------------------------------------------------*/
bool Job::bObjectTracker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pObjectTracker != nullptr;
}

/*----------------------------------------------------------------------------*/
/* print contents of Object tracker                                           */
/*----------------------------------------------------------------------------*/
void Job::vObjectTrackerDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  if(bObjectTracker())
  {
    m_pObjectTracker->vDump(m_sJobNm);
  }
}
/*----------------------------------------------------------------------------*/
/* begin output processor                                                     */
/*----------------------------------------------------------------------------*/
void Job::vOutputBeginProc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pOutput->vBeginProcOneCmd();
}

/*----------------------------------------------------------------------------*/
/* create output processor                                                    */
/*----------------------------------------------------------------------------*/
void Job::vOutputCreate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(! m_pOutput);
  
  m_pOutput = new Output;
}

/*----------------------------------------------------------------------------*/
/* destroy output processor                                                   */
/*----------------------------------------------------------------------------*/
void Job::vOutputDestroy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  delete m_pOutput;
  m_pOutput = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set if output is enabled                                                   */
/*----------------------------------------------------------------------------*/
void Job::vOutputEnabled(bool bEnabled)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* get origin job id                                                          */
/*----------------------------------------------------------------------------*/
JobId Job::tOriginId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_tOriginId;
}

/*----------------------------------------------------------------------------*/
/* set origin job id                                                          */
/*----------------------------------------------------------------------------*/
void Job::vOriginId(JobId tOriginId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tOriginId = tOriginId;
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginNodeId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  i8 iOriginNodeId = m_tOriginId.uNodeId();
  
  return iOriginNodeId;
}

/*----------------------------------------------------------------------------*/
/* get the parent originating job (on origin only)                            */
/*----------------------------------------------------------------------------*/
Job* Job::pOriginJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
      
  //RASSERT(bIsOnOrigin());
  
  Job* pOriginJob = AM::OLIST::pFindJob(m_tOriginId);
  
  return pOriginJob;
}

/*----------------------------------------------------------------------------*/
/* Notify job of node failure (on origin only)                                */
/*----------------------------------------------------------------------------*/
void Job::vOriginJobNodeFailure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(bIsOnOrigin());

  vJobError(EC_NODE_LOSS);
  vNodeLoss(true);
}

/*----------------------------------------------------------------------------*/
/* get name of node this came from                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::szOriginNodeNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 i_Node = iOriginNodeId();
  
  Stz* szNodeNm = AM::pGrid()->pFindNodeById(i_Node)->szNodeNm();
  
  return szNodeNm;
}

/*----------------------------------------------------------------------------*/
/* get parent job id                                                          */
/*----------------------------------------------------------------------------*/
JobId Job::tParentId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_tParentId;
}

/*----------------------------------------------------------------------------*/
/* set parent job id                                                          */
/*----------------------------------------------------------------------------*/
void Job::vParentId(JobId tParentId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tParentId =tParentId;
}

/*----------------------------------------------------------------------------*/
/* get parent job                                                             */
/*----------------------------------------------------------------------------*/
Job* Job::pParentJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  Job* pParentJob = AM::JLIST::pFindJob(m_tParentId);

  return pParentJob;
}

/*----------------------------------------------------------------------------*/
/* check if Job is a persistent transaction                                   */
/*----------------------------------------------------------------------------*/
bool Job::bPersistent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_bPersistentTx;
}

/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Job::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Task* pTask = this->pTask();
  LogicUnit* pLogicUnit = pTask->pLogicUnit();

  Pipeline* pPipeline = (* pLogicUnit->m_xpPipeline);
    
  return pPipeline;
}

/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Job::pMetricsPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pMetricsPipeline;
}

/*----------------------------------------------------------------------------*/
/* set pipeline                                                               */
/*----------------------------------------------------------------------------*/
void Job::vMetricsPipeline(Pipeline* pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pMetricsPipeline = pPipeline;
}

/*----------------------------------------------------------------------------*/
/* pipeout the response operand                                               */
/*----------------------------------------------------------------------------*/
void Job::vPipeoutResponse(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  // get the parent job
  Job* pParentJob = this->pParentJob();

  // parentJob
  if (pParentJob) {

    // does parent job have a responseOp ?
    if (pParentJob->m_p_ResponseOp) {

      // get the current task
      Task *pCurTask = MY::pTask();

      // is it an EXIT MARKER ?
      if (pOperand->bIsMarker()) {
        Marker* pMarker = _pMarker(pOperand);

        if (pMarker->eOpMarker() == OM_INTERNALERROR) {
          //vTrace("PARENT RESPONSE: INTERNAL ERROR IGNORED\n"_Stz);
          pParentJob->m_p_ResponseOp->vPipeToThis(pOperand, pCurTask);
          return;
        }

        if (pMarker->eOpMarker() == OM_LOWMEMORY) {
          //vTrace("PARENT RESPONSE: LOWMEMORY IGNORED\n"_Stz);
          pParentJob->m_p_ResponseOp->vPipeToThis(pOperand, pCurTask);
          return;
        }


        if (pMarker->eOpMarker() == OM_EXIT) {
          AM::vDeleteOperand(pOperand, nullptr);
          pOperand = AM::MARKER::pNewMarker(OM_CHILDEXIT, pParentJob);
        }
      }

      // pipe the operand to the responseOP of parentJob
      pParentJob->m_p_ResponseOp->vPipeToThis(pOperand, pCurTask);

    } else {

      // is it an EXIT MARKER ?

      if (pOperand->bIsMarker()) {
        Marker* pMarker = _pMarker(pOperand);

        if (pMarker->eOpMarker() == OM_EXIT) {
          AM::vDeleteOperand(pOperand, nullptr);
          pOperand = AM::MARKER::pNewMarker(OM_CHILDEXIT, pParentJob);
        }
      }

      // pipe the operand to the parent job
      pParentJob->vPipeoutResponse(pOperand);
    }
    return;
  }

  // does the job have an output object ?
  if (m_pOutput) {

    // pipe operand to the output object
    m_pOutput->vPipeToThis(pOperand);
  }
  else {

    // nowhere to go, just print response
    vTrace("Job sending response (operand): %s\n"_Stz, aCN[pOperand->eGetClassNo()]);
  }
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Job::vPostRecv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  m_okLock.vResetLock();
  m_okLock.vResetStats();

  if(bHasTag(C_TAG_PERSISTENT))
    vEnablePersistence();
  // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
  // originating job
  m_pExecuteJob  = nullptr;
  m_pEndJob  = nullptr;
  m_bOriginatorJob = false;

  // - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
  // participant job
  m_pBegunJob  = nullptr;
  m_pExecutedJob  = nullptr;
  m_pEndedJob  = nullptr;
  m_pCancelledJob  = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // First set all nullptr's then allocate as allocate mail fail
  // and we try to cleanup an invalid pointer which was recvd over the socket

  m_pRejectJob = nullptr;

  /* 00 */ m_noActiveOps.vResetAtomic();
  /* 01 */ m_eJobList     = JL_NONE;
  /* 02 */ m_pJobMetrics  = nullptr;
  /* 03 */ m_pMutex       = nullptr;
  /* 04 */ m_pOutput      = nullptr;
  /* 05 */ m_p_ResponseOp = nullptr;
  /* 06 */ m_oResponseTimer.vStart();   // vReset
  /* 07 */ m_pTask        = nullptr;
  /* 08 */ m_bWasReceived = true;
  /* 19 */ m_pMarkerChain = nullptr;
  /* 10 */ m_bAllocate    = true;
  /* 11 */ m_pObjectTracker = nullptr;
  /* 11 */ m_pJobsSkipNode    = nullptr;
  /* 12 */ m_pGenericSkipNode = nullptr;

  m_tJobId.vNextJobId();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Now allocate the members of the job for this node after recv

  m_pRejectJob = new RejectJob;

  /* 00 */ m_noActiveOps.vResetAtomic();
  /* 03 */ m_pMutex       = new Mutex(this);
  /* 07 */ m_pTask        = m_pTask = new Task(szJobNm(),this);
  /* 11 */ m_pJobsSkipNode    = new(C_SKIPLIST_LEVELS,nullptr) SkipNode(this);
  /* 12 */ m_pGenericSkipNode = new(C_SKIPLIST_LEVELS,nullptr) SkipNode(this);

  m_pTask->vSetInxPtr("job::entryPoint"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::JLIST::vInsertJob(this);

  if (AM::SYSARGS::bDistJob())
    vJobTrace("Job"_Stz, "vPostRecv"_Stz, "#rcv"_Stz,""_Stz);
  
  if (bHasTag(C_SEE_JOB))
    vPrintJobContents();
}

/*----------------------------------------------------------------------------*/
/* pipeout the operand                                                        */
/*----------------------------------------------------------------------------*/
void Job::vProcessOutput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(m_pOutput);
  m_pOutput->vProcessBufferQueue();
}

/*----------------------------------------------------------------------------*/
/* see job dump                                                               */
/*----------------------------------------------------------------------------*/
void Job::vPrintJobContents(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  SP_Gid spJobId;
  SP_Timer spTimer;

  SP_Pointer spvPipeline;
  SP_Pointer spvSingularOp;
  SP_Pointer spvOriginatingTask;
  SP_Pointer spvTask;
  SP_Pointer spvJobLogicUnit;

  Task* pTask = this->pTask();
  LogicUnit* pLogicUnit = pTask->pLogicUnit();

  vTrace("See Job %p::\n"_Stz,this);

  vTrace(
         
         "            sJobId "  "%s"    " \n"  // 0
         "            sJobNm "  "%s"    " \n"  // 1

         "              sCmd "  "%s"    " \n"  // 3
         
         "     iOriginNodeId "  "%lld"  " \n"  // 4

         "            iCmdNo "  "%lld"  " \n"  // 6

         "      bWasReceived "  "%s"    " \n"  // 7

         "         eJobState "  "%s"    " \n"  // 10
         "         eJobPhase "  "%s"    " \n"  // 11
         "          eJobList "  "%s"    " \n"  // 12
         
         "            oTimer "  "%s"    " \n"  // 13
         "            eError "  "%s"    " \n"  // 15

 
         "             pTask "  "%s"    " \n"  // 21
        
         "        pLogicUnit "  "%s"    " \n"  // 22

         ""_Stz
         
         ,spJobId.sz(m_tJobId.iAsI8())     // 0
         ,m_sJobNm.szStz()           // 1
         ,szCmd()                    // 3
         ,iOriginNodeId()            // 4

         ,m_tParentId.uJobNo()          // 6
         ,m_bWasReceived ? "true" : "false" // 7
         ,aMS[eMutexSide()]           // 10
         ,aJP[m_eJobPhase]           // 11
         ,aJL[m_eJobList]            // 12
         ,spTimer.sz(& m_oResponseTimer)     // 13
         ,aEC[m_eError_job]          // 15
         ,spvTask.sz(m_pTask)        // 21
         ,spvJobLogicUnit.sz(pLogicUnit) // 22
         
         );
  
  m_oJobTags.vContentDump();

  vTrace(";;\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Job::vPrintJobLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  SP_Gid spJobGid;
      
  vTrace("  Job %s %s "_Stz
         ,spJobGid.sz(this->tJobId().iAsI8())
         ,this->szJobNm()
         );

  vTrace("originNodeId %lld command %lld "_Stz
         ,this->iOriginNodeId()
         ,this->m_tParentId.uJobNo()
         );

  if (C_UNKNOWN != this->iDistNodeId())
    vTrace("partNode %lld "_Stz
           ,this->iDistNodeId()
           );

  if (m_pTask) {

    LogicUnit* pLogicUnit = m_pTask->pLogicUnit();

    if (pLogicUnit) {
      if (pLogicUnit->m_xpVertexSpec.bIsNotNull())
        vTrace("%s "_Stz
               ,pLogicUnit->m_xpVertexSpec->szVertexNm()
               );
      vTrace("%s "_Stz
             ,aLH[pLogicUnit->m_eDdlLocked]
             );
    }

    vTrace("AcksRmg %lld (out of %lld)"_Stz
           ,this->pTask()->iAcksRmg_assert_only_()
           ,this->pTask()->iAcksOriginal()
           );

    vTrace("logicUnit %p task %p"_Stz
           ,pLogicUnit
           ,m_pTask
           );
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get progress timer                                                         */
/*----------------------------------------------------------------------------*/
Timer* Job::pProgressTimer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return & m_oProgressTimer;
}

/*----------------------------------------------------------------------------*/
/* Notify job of node failure (participant jobs only)                         */
/*----------------------------------------------------------------------------*/
void Job::vPtpJobNodeFailure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vJobError(EC_NODE_LOSS);

  Pipeline* pPipeline = this->pPipeline();
  pPipeline->vNodeLossAbort();

  RASSERT(bIsParticipant());
}

/*----------------------------------------------------------------------------*/
/* set reportTime                                                             */
/*----------------------------------------------------------------------------*/
void Job::vUpdateParent(bool bUpdateParent)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_bUpdateParent = bUpdateParent;
}

/*----------------------------------------------------------------------------*/
/* get reportTime                                                             */
/*----------------------------------------------------------------------------*/
bool Job::bUpdateParent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_bUpdateParent;
}

/*----------------------------------------------------------------------------*/
/* get RejectJob                                                              */
/*----------------------------------------------------------------------------*/
RejectJob* Job::pRejectJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pRejectJob;
}

/*----------------------------------------------------------------------------*/
/* reset object tracker                                                       */
/*----------------------------------------------------------------------------*/
void Job::vResetObjectTracker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if(bObjectTracker()) {
    m_pObjectTracker->vReset();
  }
}

/*----------------------------------------------------------------------------*/
/* reset output                                                               */
/*----------------------------------------------------------------------------*/
void Job::vResetOutput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(m_pOutput);
  m_pOutput->m_pOutputQueue->vReset();
  m_pOutput->vEndProcOneCmd();
}

/*----------------------------------------------------------------------------*/
/* cleanup output                                                             */
/*----------------------------------------------------------------------------*/
void Job::vCleanupOutput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //RASSERT( ! m_p_ResponseOp );

  Job* pParentJob = this->pParentJob();

  if (pParentJob) {
    pParentJob->vCleanupOutput();
  }
  else {
    RASSERT(m_pOutput);
    if (! m_pOutput->m_bToExit)
      m_pOutput->vProcessBufferQueue();

    m_pOutput->m_pOutputQueue->vReset();
    m_pOutput->vEndProcOneCmd();

    if (m_pOutput->m_pStreamDef_result) {
      StreamDef* pStreamDef = m_pOutput->m_pStreamDef_result;
      m_pOutput->m_pStreamDef_result = nullptr;
      delete pStreamDef;
    }

    m_pOutput->m_bToExit      = false;
  }
}

/*----------------------------------------------------------------------------*/
/* reset task for given job                                                   */
/*----------------------------------------------------------------------------*/
void Job::vResetTaskForJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz* szEntry = "job::entryPoint"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Task* pTask = this->pTask();
    
  pTask->vSetInxPtr(szEntry);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vResponseTimerStart();
}

/*----------------------------------------------------------------------------*/
/* get pResponseOp                                                             */
/*----------------------------------------------------------------------------*/
Op* Job::p_ResponseOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_p_ResponseOp;
}

/*----------------------------------------------------------------------------*/
/* set pResponseOp                                                            */
/*----------------------------------------------------------------------------*/
void Job::vResponseOp(Op* p_ResponseOp)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_p_ResponseOp = p_ResponseOp;
}

/*----------------------------------------------------------------------------*/
/* get elapsed time in milliseconds                                           */
/*----------------------------------------------------------------------------*/
i8 Job::iResponseTimerMsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oResponseTimer.iMilliseconds();
}

/*----------------------------------------------------------------------------*/
/* get timer start                                                            */
/*----------------------------------------------------------------------------*/
i8 Job::iResponseTimerStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oResponseTimer.iStart();
}

/*----------------------------------------------------------------------------*/
/* start/reset timer                                                          */
/*----------------------------------------------------------------------------*/
void Job::vResponseTimerStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oResponseTimer.vStart();
  m_iSysTimeStart = Hardware::uReadClock();
}

/*----------------------------------------------------------------------------*/
/* update timer                                                               */
/*----------------------------------------------------------------------------*/
void Job::vResponseTimerUpdate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oResponseTimer.vUpdate();
  m_iSysTimeEnd = Hardware::uReadClock();
}

/*----------------------------------------------------------------------------*/
/* enable/disable allocation of memory by job                                 */
/*----------------------------------------------------------------------------*/
void Job::vSetAllocate(bool bAlloc)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_bAllocate = bAlloc;
}

/*----------------------------------------------------------------------------*/
/* is system closer?                                                          */
/*----------------------------------------------------------------------------*/
bool Job::bSystemCloser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  TagSet* pJobTags = this->pJobTags();

  if (! pJobTags->bHasName(C_TAG_SYSTEMCLOSER))
    return false;
  
  if (! pJobTags->iValue(C_TAG_SYSTEMCLOSER))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set system closer?                                                         */
/*----------------------------------------------------------------------------*/
void Job::vSystemCloser(bool bSystemCloser)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  TagSet* pJobTags = this->pJobTags();

  pJobTags->vReplace(C_TAG_SYSTEMCLOSER,(i8)bSystemCloser);
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Task* Job::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Job::vTask(Task* pTask)
{
  ASSERT(! pTask || ! m_pTask);
  
  if (! pTask)
    vDebugPoint();
  
  m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* create task for job                                                        */
/*----------------------------------------------------------------------------*/
void Job::vTaskCreate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(! m_pTask);

  m_pTask = new Task(szJobNm(),this);
    
  m_pTask->vSetInxPtr("job::entryPoint"_Stz);
  
  vResponseTimerStart();
}

/*----------------------------------------------------------------------------*/
/* Track allocated object by this job                                         */
/*----------------------------------------------------------------------------*/
void Job::vTrackAllocObject(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if(bObjectTracker()) {
    m_pObjectTracker->vTrackAllocClass(eClassNo);
  }
}

/*----------------------------------------------------------------------------*/
/* Track freed object by this job                                             */
/*----------------------------------------------------------------------------*/
void Job::vTrackFreeObject(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if(bObjectTracker()) {
    m_pObjectTracker->vTrackFreeClass(eClassNo);
  }
}

/*----------------------------------------------------------------------------*/
/* get the participant tsc                                                    */
/*----------------------------------------------------------------------------*/
i8 Job::iTscBeginning(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscBeginning;
}

/*----------------------------------------------------------------------------*/
/* set the beginning tsc                                                      */
/*----------------------------------------------------------------------------*/
void Job::vTscBeginning(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscBeginning = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the beginning tsc                                                      */
/*----------------------------------------------------------------------------*/
i8 Job::iTscBegun(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscBegun;
}

/*----------------------------------------------------------------------------*/
/* set the begun tsc                                                          */
/*----------------------------------------------------------------------------*/
void Job::vTscBegun(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscBegun = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the canceled tsc                                                       */
/*----------------------------------------------------------------------------*/
i8 Job::iTscCanceled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscCanceled;
}

/*----------------------------------------------------------------------------*/
/* set the canceled tsc                                                       */
/*----------------------------------------------------------------------------*/
void Job::vTscCanceled(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscCanceled = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the cancelling tsc                                                     */
/*----------------------------------------------------------------------------*/
i8 Job::iTscCanceling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscCanceling;
}

/*----------------------------------------------------------------------------*/
/* set the canceling tsc                                                      */
/*----------------------------------------------------------------------------*/
void Job::vTscCanceling(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscCanceling = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the ended tsc                                                          */
/*----------------------------------------------------------------------------*/
i8 Job::iTscEnded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscEnded;
}

/*----------------------------------------------------------------------------*/
/* set the ended tsc                                                          */
/*----------------------------------------------------------------------------*/
void Job::vTscEnded(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscEnded = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the ending tsc                                                         */
/*----------------------------------------------------------------------------*/
i8 Job::iTscEnding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscEnding;
}

/*----------------------------------------------------------------------------*/
/* set the ending tsc                                                         */
/*----------------------------------------------------------------------------*/
void Job::vTscEnding(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscEnding = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the executed tsc                                                       */
/*----------------------------------------------------------------------------*/
i8 Job::iTscExecuted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscExecuted;
}

/*----------------------------------------------------------------------------*/
/* set the executed tsc                                                       */
/*----------------------------------------------------------------------------*/
void Job::vTscExecuted(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscExecuted = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the executing tsc                                                      */
/*----------------------------------------------------------------------------*/
i8 Job::iTscExecuting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscExecuting;
}

/*----------------------------------------------------------------------------*/
/* set the executing tsc                                                      */
/*----------------------------------------------------------------------------*/
void Job::vTscExecuting(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscExecuting = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the participant tsc                                                    */
/*----------------------------------------------------------------------------*/
i8 Job::iTscParticipant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscParticipant;
}

/*----------------------------------------------------------------------------*/
/* set the participant tsc                                                    */
/*----------------------------------------------------------------------------*/
void Job::vTscParticipant(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscParticipant = iTsc;
}

/*----------------------------------------------------------------------------*/
/* verify job is composed correctly and therefore capable of being executed   */
/*----------------------------------------------------------------------------*/
void Job::vVerifyJobComposition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);

  RASSERT(EC_NONE == eValidHeader());
  RASSERT(CN_JOB == eGetClassNo());
  aFooter()->vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* get was received flag                                                      */
/*----------------------------------------------------------------------------*/
bool Job::bWasReceived(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bWasReceived;
}

/*----------------------------------------------------------------------------*/
/* set ws received flag                                                       */
/*----------------------------------------------------------------------------*/
void Job::vWasReceived(bool bWasReceived)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bWasReceived = bWasReceived;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
