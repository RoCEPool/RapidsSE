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


/*                      IMPORTANT NOTE
 
    If you add a pointer member into this class then modify
      JOBS_fn(vJob_postRecv) to set it to null when the job is received
 
 */

namespace STORE
{
typedef ListExo_t<CN_STMTLIST, Statement> StmtList;

  class Job : public Operand,
              public CRM<Job>
  {
  private:
    Lock m_okLock;
    
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // universally immutable information                    UNIVERSAL

    JobId  m_tJobId;      // this job's id
    JobId  m_tParentId;   // job id of parent
    JobId  m_tOriginId;   // origin id
    JobId  m_tDistId;     // distributed id
    i8     m_iForkNodeId; // node where job was forked

    s32    m_sJobNm;      // name of job

    Atomic m_noActiveOps; // number of active ops

    // -- command and tags
    TagSet m_oJobTags;  // tags for the job

    
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
  
    // information depending on job instance/location           MODAL

    // -- postage
    b8 m_bWasReceived;   // was job received over a socket?          POST_RECV
  
    // -- state
    eJP m_eJobPhase;  // current phase: JP_BEGINNING, _BEGUN, ...
    eJL m_eJobList;   // current list : JL_ORIGINATING, ...            POST_RECV

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // job running as originator on origin                 ORIGINATOR

    Op*   m_p_ResponseOp;    // operator to where responses go         POST_RECV
    Timer m_oResponseTimer;  // timer (on origin only)
  
    eEC   m_eError_job;      // error when starting job
                             // DOUBLE CHECK THIS BELONGS HERE

    Output*  m_pOutput_job;  // output processor                       POST_RECV

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // job running on participant node                    PARTICIPANT
 
  
    // -- tasking
    Task* m_pTask;  // task executing this job                         POST_RECV
  
    s4K m_sEntryPoint;   // sen entry code
  
    s256 m_sFolderPath;   // folder path
  
    i8  m_iTscParticipant;

    i8  m_iTscBeginning;
    i8  m_iTscBegun;
    i8  m_iTscExecuting;
    i8  m_iTscExecuted;
    i8  m_iTscEnding;
    i8  m_iTscEnded;
    i8  m_iTscCanceling;
    i8  m_iTscCanceled;

 
    b8  m_bReapable;  // can job be reaped?
  
    JobMetrics* m_pJobMetricsChain;                                // POST_RECV


    Mutex* m_pMutex;                                               // POST_RECV

    // TO BE MOVED
    Timer m_oProgressTimer; // move to register

    StmtList* m_pStmtList;    // list of cascade statements
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Job(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId);

   ~Job(void) noexcept(false);
  
    static void vPostRecv(void* aJob);
    static void vPrintJobNode(SkipNode* pNode, int i_Level);
  
    i8   nActiveOps(void);
    void vActiveOpsDec(void);
    void vActiveOpsInc(void);

    void vCascadeReset(void);
    Stz* szCmd(void);
    void vCmd(Stz* szCmd);

    JobId tDistId(void);
    void vDistIdGenerate(void);
    void vDotPrint(Stz* szText);
    void vDropLock(void);

    Stz* szEntryPoint(void);
    void vEntryPoint(Stz* szEntry);
  
    Stz* szFolderPath(void);
    void vFolderPath(Stz* szFolderPath);
    i8   iForkNodeId(void);
    void vForkNodeId(i8 iForkNodeId);

    void vGrabLock(void);
  
    bool bHasByPromptTag(void);
    bool bHasStmtList(void);
    bool bHasTag(Stz* szTagNm);
  
    bool bIsInError(void);
    bool bIsOnOrigin(void);
    bool bIsOriginator(void);
    bool bIsParticipant(void);
    bool bIsPipeline(void);
    bool bIsPipelineLocal(void);
  
    void vJobError(eEC eErrorCode);
    eEC  eJobError(void);
    JobId tJobId(void);
    eJL  eJobList(void);
    void vJobList(eJL eJobList);
    void vJobMetricsAdd(JobMetrics* pJobMetrics);
    JobMetrics* pJobMetricsChain(void);
    Stz* szJobNm(void);
    void vJobNm(Stz* szJobNm);
    eJP  eJobPhase(void);
    void vJobPhase(eJP eJobPhase);
    TagSet* pJobTags(void);
    void vJobTrace(Stz* szAtCmd, Stz* szAtRoutine, Stz* szAtPoint);

    JobId tLastJobId(void);
    void  vLastJobId(JobId tLastJobId);

    void vMutexCreate(void);
    void vMutexDestroy(void);
    void vMutexLock(void);
    eMS  eMutexSide(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);

    Output* pOutput(void);
    void vOutput(Output* pOutput);
    void vOutputBeginProc(void);
    void vOutputCreate(void);
    void vOutputDestroy(void);
    void vOutputEnabled(bool bEnabled);
    void vOutputEndProc(void);
    JobId tOriginId(void);
    void vOriginId(JobId tOriginId);
    Job* pOriginJob(void);
    i8   iOriginNodeId(void);
    Stz* szOriginNodeNm(void);
  
    JobId tParentId(void);
    void vParentId(JobId tParentId);
    Job* pParentJob(void);
    Pipeline* pPipeline(void);
    void vPrintJobContents(void);
    void vPrintJobLine(void);
    void vPostRecv(void);
    Timer* pProgressTimer(void);
  
    bool bReapable(void);
    void vReapable(bool bReapable);
    void vReapableJob(void);
    void vResetTaskForJob(void);
    Op*  p_ResponseOp(void);
    void vResponseOp(Op* p_ResponseOp);
    i8   iResponseTimerMsec(void);
    i8   iResponseTimerStart(void);
    void vResponseTimerStart(void);
    void vResponseTimerUpdate(void);

    void vStmtListAppend(Stz* szCascade);
    void vStmtListCreate(void);
    void vStmtListDestroy(void);
    void vStmtListPop(String* pString);
    bool bSystemCloser(void);
    void vSystemCloser(bool bSystemCloser);

    Task* pTask(void);
    void vTask(Task* pTask);
    void vTaskCreate(void);
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
