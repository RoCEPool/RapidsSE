/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

static Stz* C_TAG_NOWAIT = "</noWait/>"_Stz;
static Stz* C_TAG_TIME   = "</time/>"_Stz;

namespace Jobs
{
  class Job : public Focal,
              public JobResult,
              public CRM<Job>
  {
  public:
  
    JobClass* m_aJobClass; // job class
  
    ReqTuple  m_xReqTuple; // the request tuple

    i8   m_iParentId;    // job id of parent job which spawned this JK_CHILD job
    
    bool m_bByPrompt;    // job was spawned by se> prompt?
    bool m_bWasReceived; // job was received over a socket

    EnameSet m_oEnameSet;  // ename-value set

    eJT  m_eJobType;     // type of job
    eJK  m_eJobKind;     // kind of job
    eJL  m_eJobList;     // which job list the job is currently in
    eJP  m_eJobPhase;    // which job phase the job is currently in
    eJS  m_eJobState;    // current state of the job
    
    Atomic  m_ixErrors;
  
    Task* m_pOriginatingTask; // originating task of the request
    i8    m_iOriginatingId;   // job id of originating job

    i8  m_iParticipantNode;    // participant node no (FromNodeNo?)
    
    Pipeline*  m_pPipeline;    // pipeline
    Op*        m_pOp;          // Op this job is executing
    Operand*   m_pOperand;
        
  private:
    Decider m_oAcksRmg_2pc;  // acks remaining for coordinating 2pc job 
    eSS     m_eSeed;         // seed to spawn
    i8      m_i_Slot;        // slot on its node this job processes
    Task*   m_pTaskJ;        // task executing the job

  public:
    Job(eCN eClassNo);
    Job(JobClass* aJobClass, ReqTuple* aReqTuple);
    Job(Job* pParentJob, JobClass* aJobClass, ReqTuple* aReqTuple);
   ~Job(void) noexcept(false);
  
    static Job* pThisTLSJob(void);

    static void vDeleteJobViaClass(Job* pJob);
    static bool bIsForeign(i8 iJobId);
    static i8   iJobNo(i8 iJobId);
    static i8   iNodeNo(i8 iJobId);
    static i8   iOriginatorNode(i8 iJobId);
    static i8   iAsNodeNo_Bits(i8 iNodeNo);
  
    static EnameSet* pJobInfo_enameSet(Stz* szJobNm);

    i8   iAcksOriginal(void);
    i8   iAcksRmg_printOnlyValue(void);
    void vAcksRmg(i8 nAcks);
    bool bAcksRmgDec(void);
    void vContentDump(void);
    void vFocalOutline(s4K& sOutline);
    bool bHasEnameTag(Stz* szEnameTag);
    i8   iJobId(void);
    EnameSet* pJobInfoEnameSet(void);
    void vLockJobMutex(void);
    void vMarkJob(void);
    void vMarkJob(Stz* szFormat, ...);
    bool bNoWaitTag(void);
    void vSignalJobMutex(void);
    void vUnlockJobMutex(void);
    void vWaitJobMutex(void);
    Stz* szNodeNm(void);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
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
    bool bTimeTag(void);

  protected:
    //void vShow(s48K& s);
  
  private:
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
  Job* _pJob(i8 iAddr);
  bool _bIsValidJob(eCN eClass);

  ////////////////////////////////////////////////////////////////////////////////
  eEC  eJob_checkCluster(Session* pSession);
  void vJob_recv(Job* pJob);
  void vJob_send(Job* pJob);
  void vJob_post(Job* pJob);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

typedef void* (*pFUN)(void*);

//typedef pFUN (*pJOBCLASSFUN)(JobClass** );

typedef JobClass* (*pJOBCLASSFUN)(pFUN** );

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
