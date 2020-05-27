/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job : public Focal,
              public CRM<Job>
  {
  private:
    JobClass* m_aJobClass;    // job class THIS WILL GO
    s32       m_sJobNm;       // name of job
    TagSet    m_oJobTags;     // tags for the job
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
    Result    m_xResult;      // result of this job
    
  
  public:

    // into task? -------------
    Pipeline*  m_pPipeline;   // pipeline
    Op*        m_pOp;         // Op this job is executing
    Operand*   m_pOperand;

    // undecided --------------
    ReqTuple  m_xReqTuple;    // the request tuple
    Atomic    m_ixErrors;
  
    Task*     m_pOriginatingTask; // originating task of the request
    i8        m_iOriginatingId;   // job id of originating job
    
  private:
    Decider m_oAcksRmg_2pc;   // acks remaining for coordinating 2pc job
    eSS     m_eSeed;          // seed to spawn
    i8      m_i_Slot;         // slot on its node this job processes
    Task*   m_pTaskJ;         // task executing the job

  public:
    Job(JobClass* aJobClass, ReqTuple* aReqTuple);

    // FIND CASE -- TO BE REMOVED
    Job(eCN eClassNo);

   ~Job(void) noexcept(false);
  
    static Job* pThisTLSJob(void);

    static void vDeleteJobViaClass(Job* pJob);
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
    void vCopyTags(JobClass* aJobClass);
    void vFocalOutline(s4K& sOutline);
    bool bHasTag(Stz* szEnameTag);
    void vIncRowsProcessed(void);
    void vIncRowsInvalid(void);
    i8   iJobId(void);
    JobClass* aJobClass(void);
    void vJobClass(JobClass* aJobClass, char* azCaller);
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
    bool bNoWaitTag(void);
    bool bParticipantHasError(void);
    void vParticipantHasError(bool bHasError);
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
    i8   i_Slot(void);
    void vSlot(i8 i_Slot);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vTimerStart(void);
    void vTimerUpdate(void);
    bool bTimeTag(void);
    bool bWasReceived(void);
    void vWasReceived(bool bWasReceived);

  protected:
    //void vShow(s48K& s);
  
  private:
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
  Job* _pJob(i8 iAddr);
  bool _bIsValidJob(eCN eClass);

  ////////////////////////////////////////////////////////////////////////////////
  eEC  eJob_checkCluster(void);
  void vJob_recv(Job* pJob);
  void vJob_send(Job* pJob);
  void vJob_post(JobClass* aJobClass, Job* pJob);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

typedef void* (*pFUN)(void*);

//typedef pFUN (*pJOBCLASSFUN)(JobClass** );

typedef JobClass* (*pJOBCLASSFUN)(pFUN** );

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
