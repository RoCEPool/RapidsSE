/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class JobClass_o;    // forward definition
  struct JobClass_s; // forward definition

  class Job;
  
  static const i8 C_MAX_COLUMNS = 128;
  
  enum eJK // job kind
  {
  /*  0 */ JK_NONE=0,
  /*  1 */ JK_CHILD,     // subordinate helper run on originator
  /*  2 */ JK_DIST2PC,   // distributed two-phase jobs
  /*  3 */ JK_GENERIC,   // the generic jobs for two-phase messaging
  /*  4 */ JK_GRANDKID,  // subordinate helper run on participant
  /*  5 */ JK_LOCAL,     // task is run on the local node
  /*  6 */ JK_SYSTEM,    // system task (such as socket receiver)
  /*  7 */ JK_THREAD,    // task is run by calling thread
  };
  
  static Stz* aJK[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CHILD"_Stz,
  /*  2 */ "DIST2PC"_Stz,
  /*  3 */ "GENERIC"_Stz,
  /*  4 */ "GRANDKID"_Stz,
  /*  4 */ "LOCAL"_Stz,
  /*  6 */ "SYSTEM"_Stz,
  /*  7 */ "THREAD"_Stz,
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
    i8  m_iJobId;              // job id
    JobClass_s* m_pJobClass_s; // job class
    eSS  m_eSeed;              // task seed
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
    void vSeed(eSS eSeed);
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    eJT  eJobTrace(void);
    void vJobTrace(eJT eJobTrace);
  
  protected:
    void vShow(s48K& s);
  
  private:
    void vInit(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
  
  void vJob_recv(Job* pJob);
  void vJob_send(Job* pJob);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

typedef void* (*pFUN)(void*);

//typedef pFUN (*pJOBCLASSFUN)(JobClass_s** );

typedef JobClass_s* (*pJOBCLASSFUN)(pFUN** );

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
