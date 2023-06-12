/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

#include "eJL.h"
#include "eMS.h"
#include "eJP.h"
#include "eRS.h"

#include "JobMetrics.hpp"
#include "JobId.hpp"
#include "Output.hpp"
#include "Statement.hpp"
#include "MarkerChain.hpp"
#include "VlText.hpp"

namespace STORE
{
typedef ListExo_t<CN_STMTLIST, Statement> StmtList;

  class Job : public Operand,
              public CRM<Job>
  {
  private:
  
    Lock   m_okLock;      // lock on this job
    
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

    JobId  m_tJobId;      // this job's id
    JobId  m_tParentId;   // id of parent job
    JobId  m_tOriginId;   // id when job is originator
    JobId  m_tDistId;     // id when job is distributed participant

    s32    m_sJobNm;      // name of job
    s4K    m_sEntryPoint; // sen entry code
    TagSet m_oJobTags;    // tags for the job
    s256   m_sFolderPath; // folder path
  
    eJP    m_eJobPhase;   // current phase: JP_BEGINNING, _BEGUN, ...
    eEC    m_eError_job;  // error when starting job
    eEC    m_eChildError; // error in child job

    bool   m_bOriginatorJob;   // is this the originator job ?

    bool   m_bNodeLoss;     // node loss detected
    bool   m_bUpdateParent; // update parent
    bool   m_bPersistentTx; // persistent job transaction flag

    bool   m_bJobMetrics;

    Pipeline* m_pMetricsPipeline;

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // originating job
    Job*  m_pExecuteJob;
    Job*  m_pEndJob;

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // participant job
    Job*  m_pBegunJob;
    Job*  m_pExecutedJob;
    Job*  m_pEndedJob;
    Job*  m_pCancelledJob;

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // reject job
    RejectJob*  m_pRejectJob;

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // reset in Job::vPostRecv()
  
    /* 00 */ Atomic  m_noActiveOps;    // number of active ops
    /* 01 */ eJL     m_eJobList;       // current list : JL_ORIGINATING, ...
    /* 02 */ JobMetrics* m_pJobMetrics;// job metrics chain
    /* 03 */ Mutex*  m_pMutex;         // mutex
    /* 04 */ Output* m_pOutput;        // output processor
    /* 05 */ Op*     m_p_ResponseOp;   // operator to where responses go
    /* 06 */ Timer   m_oResponseTimer; // timer (on origin only)
    /* 07 */ Task*   m_pTask;          // task executing this job
    /* 08 */ b8      m_bWasReceived;   // was job received over a socket?
    /* 09 */ MarkerChain* m_pMarkerChain; // chain of free markers
    /* 10 */ b8      m_bAllocate;      // can job ask for memory?
    /* 11 */ ObjectTracker* m_pObjectTracker;// Frequency count of object types allocated/freed by this Job
    /* 12 */ SkipNode* m_pJobsSkipNode;     // Jobs list skip node
    /* 13 */ SkipNode* m_pGenericSkipNode;  // generic list skip node

    /* 14 */ VlText* m_pCmdText;    // code text experiment

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

    i8  m_iTscParticipant;

    i8  m_iTscBeginning;
    i8  m_iTscBegun;
    i8  m_iTscExecuting;
    i8  m_iTscExecuted;
    i8  m_iTscEnding;
    i8  m_iTscEnded;
    i8  m_iTscCanceling;
    i8  m_iTscCanceled;

    i8 m_iSysTimeStart;
    i8 m_iSysTimeEnd;

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

    // TO BE MOVED
    Timer m_oProgressTimer; // move to register

    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Job(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId, TagSet* pCmdTagSet = nullptr);

   ~Job(void) noexcept(false);

    static JobId tDistId(Job* pJob);
    static Stz*  szJobNm(Job* pJob);
    static JobId tOriginId(Job* pJob);
    static void vPostRecv(void* aJob);
    static void vPrintJobNode(SkipNode* pNode, int i_Level);
  
    i8   nActiveOps(void);
    void vActiveOpsDec(void);
    void vActiveOpsInc(void);
    bool bAllocateMemory(void);

    Job* pBegunJob(void);
    void vBegunJob(Job* pBegunJob);

    Job* pCancelledJob(void);
    void vCancelledJob(Job* pCancelledJob);
    eEC eChildError(void);
    void vChildError(eEC eErrorCode);
    void vCleanupOutput(void);
    Stz* szCmd(void);
    void vCmd(Stz* szCmd);
    void vDisableObjectTracker(void);
    JobId tDistId(void);
    void vDistIdGenerate(void);
    i8   iDistNodeId(void);
    void vDotPrint(Stz* szText);
    void vDrainOutput(void);
    void vDropLock(void);

    void vEnableObjectTracker(void);
    void vEnablePersistence(void);
    Job* pEndJob(void);
    void vEndJob(Job* pEndJob);
    Job* pEndedJob(void);
    void vEndedJob(Job* pEndedJob);
    Stz* szEntryPoint(void);
    void vEntryPoint(Stz* szEntry);
    Job* pExecuteJob(void);
    void vExecuteJob(Job* pExecuteJob);
    Job* pExecutedJob(void);
    void vExecutedJob(Job* pExecutedJob);

    Stz* szFolderPath(void);
    void vFolderPath(Stz* szFolderPath);
    void vFreeMarker(Marker* pMarker);

    SkipNode* pGenericSkipNode(void);
    void vGetTimerFromChild(Job* pChildJob);
    i8   iGetSysTimerStart(void);
    i8   iGetSysTimerEnd(void);
    void vGrabLock(void);

    bool bHasTag(Stz* szTagNm);
  
    bool bIsInError(void);
    bool bIsOnOrigin(void);
    bool bIsOriginator(void);
    bool bOriginatorJob(void);
    void vOriginatorJob(bool bOriginatorJob);
    bool bIsParticipant(void);
    bool bIsPipeline(void);
    bool bIsPipelineLocal(void);
  
    void vJobError(eEC eErrorCode);
    eEC  eJobError(void);
    JobId tJobId(void);
    eJL  eJobList(void);
    void vJobList(eJL eJobList);
    void vJobMetrics(bool bJobMetrics);
    bool bJobMetrics(void);
    void vJobMetricsAdd(JobMetrics* pJobMetrics);
    JobMetrics* pJobMetricsChain(void);
    Stz* szJobNm(void);
    void vJobNm(Stz* szJobNm);
    eJP  eJobPhase(void);
    void vJobPhase(eJP eJobPhase);
    SkipNode* pJobsSkipNode(void);
    TagSet* pJobTags(void);
    void vJobTrace(Stz* szAtCmd, Stz* szAtRoutine, Stz* szAtPoint, Stz* szNote);

    void  vLabelObjectTracker(Stz* szLabel);
    JobId tLastJobId(void);
    void  vLastJobId(JobId tLastJobId);

    void vMutexCreate(void);
    void vMutexDestroy(void);
    void vMutexLock(void);
    eMS  eMutexSide(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);

    Marker* pNewMarker(eOM eOpMarker);
    void vNewMarkerChain(void);

    bool  bNodeLoss(void);
    void  vNodeLoss(bool bNodeLoss);

    bool bObjectTracker(void);
    void vObjectTrackerDump(void);
    void vOutputBeginProc(void);
    void vOutputCreate(void);
    void vOutputDestroy(void);
    void vOutputEnabled(bool bEnabled);
    JobId tOriginId(void);
    void vOriginId(JobId tOriginId);
    Job* pOriginJob(void);
    void vOriginJobNodeFailure(void);
    i8   iOriginNodeId(void);
    Stz* szOriginNodeNm(void);
  
    JobId tParentId(void);
    void vParentId(JobId tParentId);
    Job* pParentJob(void);
    bool bPersistent(void);
    Pipeline* pPipeline(void);
    void vMetricsPipeline(Pipeline* pPipeline);
    Pipeline* pMetricsPipeline(void);
    void vPipeoutResponse(Operand* pOperand);
    void vPrintJobContents(void);
    void vPrintJobLine(void);
    void vPostRecv(void);
    void vProcessOutput(void);
    Timer* pProgressTimer(void);
    void vPtpJobNodeFailure(void);

    void vUpdateParent(bool bUpdateParent);
    bool bUpdateParent(void);

    void vRejected(bool bRejected);
    bool bRejected(void);

    RejectJob* pRejectJob(void);

    void vResetObjectTracker(void);
    void vResetOutput(void);
    void vResetTaskForJob(void);
    Op*  p_ResponseOp(void);
    void vResponseOp(Op* p_ResponseOp);
    i8   iResponseTimerMsec(void);
    i8   iResponseTimerStart(void);
    void vResponseTimerStart(void);
    void vResponseTimerUpdate(void);

    void vSetAllocate(bool bAlloc);
    bool bSystemCloser(void);
    void vSystemCloser(bool bSystemCloser);

    Task* pTask(void);
    void vTask(Task* pTask);
    void vTaskCreate(void);
    void vTrackAllocObject(eCN eClassNo);
    void vTrackFreeObject(eCN eClassNo);
    i8   iTscBeginning(void);
    void vTscBeginning(i8 iTsc);
    i8   iTscBegun(void);
    void vTscBegun(i8 iTsc);
    i8   iTscCanceled(void);
    void vTscCanceled(i8 iTsc);
    i8   iTscCanceling(void);
    void vTscCanceling(i8 iTsc);
    i8   iTscEnded(void);
    void vTscEnded(i8 iTsc);
    i8   iTscEnding(void);
    void vTscEnding(i8 iTsc);
    i8   iTscExecuted(void);
    void vTscExecuted(i8 iTsc);
    i8   iTscExecuting(void);
    void vTscExecuting(i8 iTsc);
    i8   iTscParticipant(void);
    void vTscParticipant(i8 iTsc);
    
    void vVerifyJobComposition(void);
  
    void vWaitJobMutex(void);
    bool bWasReceived(void);
    void vWasReceived(bool bWasReceived);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);

  void vJob_postRecv(Job* pJob);
}


typedef void* (*pFUN)(void*);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
