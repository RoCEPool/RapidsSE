/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job : public Focal,
              public JobDicPath,
              public JobObjectName,
              public JobResult
  {
  public:
  
    struct {
      i8   m_iRequestId; // the command which trigger the job
      i8   m_i_State;    // which session state
      bool m_bMarkTrace; // trace (mark version)
      bool m_bLogJob;    // mark activity
    };
  
    i8  m_iParentId;  // job id of parent job which spawned this JK_CHILD job
    eSS m_eSeed;      // task seed
    i8  m_i_Slot;     // slot this sub truncate truncates
    eJP m_eJobPhase;  // which phase the job is in
    eJS m_eJobState;  // job state
    eJL m_eJobList;   // which job list is job currently in?
    bool m_bNoWait;      // no wait
    bool m_bWasReceived; // job was received over a socket

    Decider m_oAcksRmg;  // acks remaining
  
    i8   m_i_ActionRoot;  // action

    JobClass*    m_pJobClass;        // job class
    Tasks::Task* m_pExecutingTask;   // task executing the job
    Tasks::Task* m_pOriginatingTask; // originating task of the request
  
    i8  m_iParticipantNode;    // participant node no (FromNodeNo?)
  
    CrwBuffer* m_pCurrentBuffer;   // row buffer
  
    Object* m_pSendWith; // object which is to be sent with this job
  
    Atomic m_ixErrors;

    Mutex* m_pMutexJob;

    eJT  m_eJobTrace;     // job trace (to be removed)

  public:
    Job(eCN eClassNo, Session* pSession, i8 iRequestId);
   ~Job(void) noexcept(false);
  
    static Job* pThisTLSJob(void);

    static bool bIsForeign(i8 iJobId);
    static i8   iJobNo(i8 iJobId);
    static i8   iNodeNo(i8 iJobId);
    static i8   iOriginatorNode(i8 iJobId);
    static i8   iAsNodeNo_Bits(i8 iNodeNo);

    void vFocalOutline(s4K& sOutline);
    Stz* sJobSeed(void);
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    eJT  eJobTrace(void);
    void vJobTrace(eJT eJobTrace);
    void vMarkJob(void);
    void vMarkJob(Stz* szFormat, ...);
    void vMutexLock(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);
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
  void vJob_post(Session* pSession, Job* pJob);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

typedef void* (*pFUN)(void*);

//typedef pFUN (*pJOBCLASSFUN)(JobClass** );

typedef JobClass* (*pJOBCLASSFUN)(pFUN** );

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
