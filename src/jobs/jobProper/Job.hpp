/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

#include "eJK.h"
#include "eJL.h"
#include "eJS.h"
#include "eJP.h"
#include "eRS.h"

namespace Jobs
{
  class Job : public Focal,
              public CRM<Job>
  {
  private:
    s32       m_sJobNm;       // name of job
    i8        m_iParentId;    // job id of parent job which spawned this job
    Timer     m_oTimer;       // timer

    bool      m_bWasReceived; // job was received over a socket
    i8        m_iParticipantNode; // participant node no (FromNodeNo?)
    bool      m_bNormalSides; // side semantics

    eJK       m_eJobKind;     // kind of job
    eJL       m_eJobList;     // which job list the job is currently in
    eJP       m_eJobPhase;    // which job phase the job is currently in
    eJS       m_eJobState;    // current state of the job

  public:
    TagSet    m_oJobTags;     // tags for the job
  
    Result    m_xResult;      // result of this job
    
    // into task? -------------
    Pipeline*  m_pPipeline;   // pipeline
    Op*        m_pOp;         // Op this job is executing
    Op*        m_pOuterOp;    // Op that called the pipeline
    Op*        m_pInputOp;    // The optional input op of the pipeline

    // undecided --------------
    ReqTuple  m_xReqTuple;    // the request tuple
    Atomic    m_ixErrors;
  
    Task*     m_pOriginatingTask; // originating task of the request
    i8        m_iOriginatingId;   // job id of originating job
    
  private:
    Decider m_oAcksRmg_2pc;   // acks remaining for coordinating 2pc job
    eSS     m_eSeed;          // seed to spawn
    i8      m_i_TablePtn;     // table ptn on its node this job processes
    Task*   m_pTaskJ;         // task executing the job

    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
    
    Job(ReqTuple* aReqTuple, TokenVec* pLitTknVec=nullptr);

    // FIND CASE -- TO BE REMOVED
    Job(eCN eClass);

   ~Job(void) noexcept(false);
  
    static Job* pThisTLSJob(void);

    static bool bIsForeign(i8 iJobId);
    static i8   iJobNo(i8 iJobId);
    static i8   iNodeNo(i8 iJobId);
    static i8   iOriginatorNode(i8 iJobId);
    static i8   iAsNodeNo_Bits(i8 iNodeNo);

    i8   iAcksOriginal(void);
    i8   iAcksRmg_printOnlyValue(void);
    void vAcksRmg(i8 nAcks);
    bool bAcksRmgDec(void);
    void vAddRowsProcessed(i8 iRowsProcessed);
    void vAddRowsInvalid(i8 iRowsInvalid);
    void vAddRowsSkipped(i8 iRowsSkipped);
    bool bByPromptTag(void);
    void vBufferQueue(Queue* pBufferQueue);
    Queue* pBufferQueue(void);
    void vContentDump(void);
    bool bDiaryTag(void);
    void vFocalOutline(s4K& sOutline);
    bool bHasTag(Stz* szEnameTag);
    bool bHasTag_bg(void);
    bool bHasTag_time(void);
    bool bHasTag_noWait(void);
    void vIncRowsProcessed(void);
    void vIncRowsInvalid(void);
    Op*  pInputOp(void);
    void vInputOp(Op* pInputOp);
    bool bIsPipeline(void);
    i8   iJobId(void);
    void vJobError(eEC eErrorCode);
    eEC  eJobError(void);
    bool bJobHasError(void);
    eJK  eJobKind(void);
    void vJobKind(eJK eJobKind);
    eJL  eJobList(void);
    void vJobList(eJL eJobList);
    Stz* szJobNm(void);
    void vJobNm(Stz* szJobNm);
    eJP  eJobPhase(void);
    void vJobPhase(eJP eJobPhase);
    eJS  eJobState(void);
    void vJobState(eJS eJobState);
    TagSet* pJobTags(void);
    void vLockJobMutex(void);
    void vMarkJob(void);
    void vMarkJob(Stz* szFormat, ...);
    void vOperandQueueReport(File* pFile, bool bStdOut);
    bool bParticipantHasError(void);
    void vParticipantHasError(bool bHasError);
    eEC ePopulateJob(TagSet* pTagSet);
    void vPrintResult(eRS eResponse);
    Response* pResponse(void);
    Stz* szResponse(void);
    void vResponse(Stz* sResponse);
    void vOriginResponseVec(ResponseVec* pResponseVec);
    ResponseVec* pOriginResponseVec(void);
    i8   iRowsProcessed(void);
    void vRowsProcessed(i8 iRowsProcessed);
    i8   iRowsSkipped(void);
    void vRowsSkipped(i8 iRowsSkipped);
    i8   iRowsInvalid(void);
    void vRowsInvalid(i8 iRowsInvalid);
    Stz* szSide(void);
    void vSignalJobMutex(void);
    void vUnlockJobMutex(void);
    void vWaitJobMutex(void);
    Stz* szNodeNm(void);
    bool bNormalSides(void);
    void vNormalSides(bool bNormalSides);
    i8   iOriginatorNode(void);
    i8   iParentId(void);
    void vParentId(i8 iParentId);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    void vPrint(Stz* szFormat, ...);
    void vReport(void);
    eSS  eSeed(void);
    Stz* szSeed(void);
    void vSeed(eSS eSeed);
    Session* pSession(void);
    void vShow(void);
    i8   i_TablePtn(void);
    void vTablePtn(i8 i_TablePtn);
    bool bSummaryTag(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    i8   iTimerMsec(void);
    void vTimerStart(void);
    void vTimerUpdate(void);
    bool bWasReceived(void);
    void vWasReceived(bool bWasReceived);

  protected:
    //void vShow(s48K& s);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
  Job* _pJob(i8 iAddr);
  bool _bIsValidJob(eCN eClass);

  ////////////////////////////////////////////////////////////////////////////////
  eEC  eJob_checkCluster(void);
  void vJob_recv(Job* pJob);
  void vJob_send(Job* pJob);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

typedef void* (*pFUN)(void*);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
