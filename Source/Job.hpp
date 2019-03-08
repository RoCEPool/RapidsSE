/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace Jobs
{
  struct JobClass; // forward definition

  class Job;
  
  static const i8 C_MAX_COLUMNS = 150;
  
  enum eJF // job flags
  {
    /*  0 */ JF_NONE   = 0x0,
    /*  1 */ JF_LOWMEM = 0x1,  // low memory sensistive
  };
  
  static Stz* aJF[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "LOWMEM"_Stz,
  };
  
  enum ePY // priority
  {
    /*  0 */ PY_NONE=0,
    /*  1 */ PY_LOW,
    /*  2 */ PY_NORMAL
  };
  
  static Stz* aPY[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "LOW"_Stz,
    /*  2 */ "NORMAL"_Stz
  };

  enum eGO // go (aka proceed)
  {
  /*  0 */ GO_NONE=0,
  /*  1 */ GO_CONTINUE,
  /*  2 */ GO_END,
  /*  3 */ GO_STALL,
  /*  4 */ GO_YIELD,
  };
  
  static Stz* aGO[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CONTINUE"_Stz,
  /*  2 */ "END"_Stz,
  /*  3 */ "STALL"_Stz,
  /*  4 */ "YIELD"_Stz,
  };
  
  enum eJK // job kind
  {
  /*  0 */ JK_NONE=0,
  /*  1 */ JK_CHILD,     // subordinate helper run on originator
  /*  2 */ JK_DIST2PC,   // distributed two-phase jobs
  /*  3 */ JK_GENERIC,   // the generic jobs for two-phase messaging
  /*  4 */ JK_GRANDKID,  // subordinate helper run on participant
  /*  5 */ JK_INFO,      // job is for info purposes only
  /*  6 */ JK_LOCAL,     // job is run on the local node
  /*  7 */ JK_SYSTEM,    // system job (such as socket receiver)
  /*  8 */ JK_THREAD,    // job is run by calling thread
  /*  9 */ JK_OTHER,     // any other
  };
  
  static Stz* aJK[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CHILD"_Stz,
  /*  2 */ "DIST2PC"_Stz,
  /*  3 */ "GENERIC"_Stz,
  /*  4 */ "GRANDKID"_Stz,
  /*  5 */ "INFO"_Stz,
  /*  6 */ "LOCAL"_Stz,
  /*  7 */ "SYSTEM"_Stz,
  /*  8 */ "THREAD"_Stz,
  /*  9 */ "OTHER"_Stz,
  };
  
  enum eJP // job phase
  {
  /*  0 */ JP_NONE=0,
  /*  1 */ JP_BEGINING,
  /*  2 */ JP_BEGUN,
  /*  3 */ JP_EXECUTING,
  /*  4 */ JP_EXECUTED,
  /*  5 */ JP_ENDING,
  /*  6 */ JP_ENDED,
  /*  7 */ JP_CANCELING,
  /*  8 */ JP_CANCELED,
  /*  9 */ JP_ENUMS
  };
  
  static Stz* aJP[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BEGINING"_Stz,
  /*  2 */ "BEGUN"_Stz,
  /*  3 */ "EXECUTING"_Stz,
  /*  4 */ "EXECUTED"_Stz,
  /*  5 */ "ENDING"_Stz,
  /*  6 */ "ENDED"_Stz,
  /*  7 */ "CANCELING"_Stz,
  /*  8 */ "CANCELED"_Stz,
  /*  9 */ "ENUMS"_Stz,
  };
  
  enum eJS // job state
  {
  /*  0 */ JS_RUNNING=0,
  /*  1 */ JS_LOCKED,
  /*  2 */ JS_WAITING,
  /*  3 */ JS_SIGNALLED
  };
  
  static Stz* aJS[]
  {
  /*  0 */ "RUNNING"_Stz,
  /*  1 */ "LOCKED"_Stz,
  /*  2 */ "WAITING"_Stz,
  /*  2 */ "SIGNALLED"_Stz,
  };
  
  enum eJT // job trace
  {
  /*  0 */ JT_NONE=0,
  /*  1 */ JT_ALL,
  /*  2 */ JT_JOB,
  /*  3 */ JT_TASK
  };
  
  static Stz* aJT[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ALL"_Stz,
  /*  2 */ "JOB"_Stz,
  /*  3 */ "TASK"_Stz
  };
  
  enum eSS // seed string
  {
  /*  0 */ SS_SEED_RUN=0,
  /*  1 */ SS_SEED_REQ,
  /*  2 */ SS_SEED_RCV,
  /*  3 */ SS_SEED_EXE,
  /*  4 */ SS_SEED_END,
  /*  5 */ SS_SEED_CAN,
  /*  6 */ SS_ENUMS
  };

  static Stz* aSS[]
  {
  /*  0 */ "SS_SEED_RUN"_Stz,
  /*  1 */ "SS_SEED_REQ"_Stz,
  /*  2 */ "SS_SEED_RCV"_Stz,
  /*  3 */ "SS_SEED_EXE"_Stz,
  /*  4 */ "SS_SEED_END"_Stz,
  /*  5 */ "SS_SEED_CAN"_Stz,
  /*  6 */ "SS_ENUMS"_Stz,
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job : public Object,
              public JobDicPath,
              public JobObjectName,
              public JobResult
  {
  public:
    i8  m_iJobId;     // job id
    i8  m_iParentId;  // job id of parent job which spawned this JK_CHILD job
    eSS m_eSeed;      // task seed
    i8  m_i_State;    // which session state
    i8  m_i_Slot;     // slot this sub truncate truncates
    eJP m_eJobPhase;  // which phase the job is in
    eJS m_eJobState;  // job state
    eJL m_eJobList;   // which job list is job currently in?
    bool m_bNoWait;      // no wait
    bool m_bWasReceived; // job was received over a socket

    Decider m_oAcksRemaining;  // acks remaining

    bool m_bMarkActivity; // mark activity
    i8   m_i_Activity;    // activity
    bool m_bMarkAction;   // mark actions
    i8   m_i_ActionRoot;  // action
    bool m_bMarkTrace;    // trace (mark version)

    JobClass*    m_pJobClass;        // job class
    Tasks::Task* m_pExecutingTask;   // task executing the job
    Tasks::Task* m_pOriginatingTask; // originating task of the request
  
    i8  m_iParticipantNode;    // participant node no (FromNodeNo?)
  
    EntryBuffer* m_pCurrentBuffer; // entry buffer
  
    Object* m_pSendWith; // object which is to be sent with this job
  
    Atomic m_ixErrors;

    Mutex* m_pMutexJob;

    eJT  m_eJobTrace;     // job trace (to be removed)

  public:
    Job(eCN eClassNo, Interface::Session* pSession);
    ~Job(void) noexcept(false);
    
    static bool bIsForeign(i8 iJobId);
    static i8   iJobNo(i8 iJobId);
    static i8   iNodeNo(i8 iJobId);
    static i8   iOriginatorNode(i8 iJobId);
    static i8   iAsNodeNo_Bits(i8 iNodeNo);
    static void vStateDiagram(s256& sDiagram, eCN eJobClass);

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
  eEC  eJob_checkCluster(Interface::Session* pSession);
  void vJob_recv(Job* pJob);
  void vJob_send(Job* pJob);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

typedef void* (*pFUN)(void*);

//typedef pFUN (*pJOBCLASSFUN)(JobClass** );

typedef JobClass* (*pJOBCLASSFUN)(pFUN** );

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
