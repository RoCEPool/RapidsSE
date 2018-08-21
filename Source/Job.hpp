/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class JobClass; // forward definition
  
  class Job;
  
  static const i8 C_MAX_COLUMNS = 128;
  
  enum eJK // job kind
  {
  /*  0 */ JK_NONE=0,
  /*  1 */ JK_CHILD,
  /*  2 */ JK_DIST2PC,
  /*  3 */ JK_GENERIC,
  /*  4 */ JK_ONESHOT,
  /*  5 */ JK_SINGLENODE,
  /*  6 */ JK_SYSTEM,
  };
  
  static Stz* aJK[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CHILD"_Stz,
  /*  2 */ "DIST2C"_Stz,
  /*  3 */ "GENERIC"_Stz,
  /*  4 */ "ONESHOT"_Stz,
  /*  5 */ "SINGLENODE"_Stz,
  /*  6 */ "SYSTEM"_Stz,
  };
  
  enum eJS // job state
  {
  /*  0 */ JS_NONE=0,
  /*  1 */ JS_BEGUN,
  /*  2 */ JS_EXECUTED,
  /*  3 */ JS_ENDED,
  /*  4 */ JS_ENUMS
  };
  
  static Stz* aJS[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "BEGUN"_Stz,
    /*  2 */ "EXECUTED"_Stz,
    /*  3 */ "ENDED"_Stz,
    /*  4 */ "ENUMS"_Stz
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
  
  const i8 C_SEED_1 = 1;
  const i8 C_SEED_2 = 2;
  const i8 C_SEED_3 = 3;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job : public Object,
              public JobDicPath,
              public JobObjectName,
              public JobResult
  {
  public:
    i8  m_iJobId;              // job id
    JobClass* m_pJobClass;     // job class
    eJS m_eState;              // job state
    s32 m_sSeed;               // task seed (to be merged with above)
    bool m_bCheckResources;    // request requires to check resources
    bool m_bDetached;          // is this job detached?
    eJT  m_eJobTrace;          // job trace
  
    i8  m_iParticipantNode;    // participant node no
    Atomic m_ziAcksRemaining;  // acks remaining
    Tasks::Task* m_pOriginatingTask;  // originating task of the request
  
    Object* m_pAncillary;      // ancilliary object
  
    WorkingBuffer* m_pWorkingBuffer; // working buffer

    Tasks::Task* m_pPromptTask;  // prompt task
    
    pthread_mutex_t     m_xThreadMutex;     // connector thread mutex variable
    pthread_mutexattr_t m_xThreadMutexAttr; // connector thread mutex shared attribute
    pthread_cond_t      m_xThreadCond;      // connector thread condition variable
    pthread_condattr_t  m_xThreadCondAttr;  // connector thread condition shared attribute

  public:
    Job(eCN eClassNo);
    Job(eCN eClassNo, Interface::SEitf* pSEitf);
    ~Job(void);
    
    static i8 iOriginatorNode(i8 iJobId);
    
    Stz* sJobSeed(void);
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    eJS  eJobState(void);
    void vJobState(eJS eJobState);
    eJT  eJobTrace(void);
    void vJobTrace(eJT eJobTrace);
  
  protected:
    void vShow(s48K& s);
  
  private:
    void vInit(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
