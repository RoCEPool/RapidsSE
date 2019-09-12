/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job : public JobBase,        // ⋗ socket
              public JobLite,        // ⋗ scheduler  ⋗ recv  ⋗ post_recv
              public JobBody,
              public JobFull,
              public JobDicPath,
              public JobObjectName,
              public JobResult
  {                                  // ⋗ parent ⋗ child ⋗ prompt ⋗ api
  public:
  
    i8   m_iParentId;    // job id of parent job which spawned this JK_CHILD job
    eSS  m_eSeed;        // task seed
    i8   m_i_Slot;       // slot this job processes
    bool m_bNoWait;      // no wait
    bool m_bWasReceived; // job was received over a socket

    eJL  m_eJobList;     // which job list the job is currently in
    eJP  m_eJobPhase;    // which job phase the job is currently in
    eJS  m_eJobState;    // current state of the job

    Decider m_oAcksRmg;  // acks remaining
  
    Tasks::Task* m_pTask;            // task executing the job
    Tasks::Task* m_pOriginatingTask; // originating task of the request
  
    i8  m_iParticipantNode;    // participant node no (FromNodeNo?)
  
    //! why do we have both of these?
    CrwBuffer* m_pCrwBuffer;   // row buffer          //! should this be in Job?
  
    Atomic m_ixErrors;

    //! to be moved into Task
    Mutex* m_pJobMutex;

    //! to be removed
    Object*    m_pSendWith; // object which is to be sent with this job

  public:
    Job(eCN eClassNo);
    Job(JobClass* aJobClass,
        ReqTuple* aReqTuple, Session* pSession);
   ~Job(void) noexcept(false);
  
    static Job* pThisTLSJob(void);

    static bool bIsForeign(i8 iJobId);
    static i8   iJobNo(i8 iJobId);
    static i8   iNodeNo(i8 iJobId);
    static i8   iOriginatorNode(i8 iJobId);
    static i8   iAsNodeNo_Bits(i8 iNodeNo);

    void vAcksRmg(i8 nAcks);
    void vFocalOutline(s4K& sOutline);
    Stz* sJobSeed(void);
    i8   iJobId(void);
    void vMarkJob(void);
    void vMarkJob(Stz* szFormat, ...);
    void vLockJobMutex(void);
    void vSignalJobMutex(void);
    void vUnlockJobMutex(void);
    void vWaitJobMutex(void);
    Stz* szNodeNm(void);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    void vReport(void);
    void vSeed(eSS eSeed);
    void vShow(void);

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
