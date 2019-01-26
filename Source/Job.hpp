/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

#define JOB_TRACE false

namespace Jobs
{
  struct JobClass; // forward definition

  class Job;
  
  static const i8 C_MAX_COLUMNS = 150;
  
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
    i8  m_iJobId;           // job id
    i8  m_i_State;          // which session state
    eJL m_eJobList;         // which job list is job currently in?
  
    JobClass* m_pJobClass;  // job class
    eSS  m_eSeed;           // task seed
    eJT  m_eJobTrace;       // job trace
    bool m_bNoWait;         // no wait
    bool m_bMark;           // write activity marks?

    Tasks::Task* m_pExecutingTask;   // task executing the job
    Tasks::Task* m_pOriginatingTask; // originating task of the request
  
    i8  m_iParticipantNode;    // participant node no
    Decider m_oAcksRemaining;  // acks remaining
  
    EntryBuffer* m_pCurrentBuffer; // entry buffer
  
    eJS m_eState; // job state
    bool m_bWasReceived; // job was received over a socket
  
    Mutex* m_pMutex;
  
  public:
    Job(eCN eClassNo);
    Job(eCN eClassNo, Interface::Session* pSession);
    ~Job(void) noexcept(false);
    
    static i8 iOriginatorNode(i8 iJobId);
    
    Stz* sJobSeed(void);
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    eJT  eJobTrace(void);
    void vJobTrace(eJT eJobTrace);
    i8   iOriginatorNode(void);
    void vMutexLock(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    void vReport(void);
    void vSeed(eSS eSeed);

  protected:
    void vShow(s48K& s);
  
  private:
    void vInit(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
  Job* _pJob(i8 iAddr);
  bool _bIsValidJob(eCN eClass);

  ////////////////////////////////////////////////////////////////////////////////
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
