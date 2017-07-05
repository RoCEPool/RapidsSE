/*============================================================================*/
/* Thread.hpp -- thread                                                       */
/*============================================================================*/
#pragma once

namespace Schedulers
{
  class Scheduler;
}

namespace Threads
{
  enum eTH // thread
  {
  /*  0 */ TH_NONE=0,
  /*  1 */ TH_ANSWER,
  /*  2 */ TH_COORDINATOR,
  /*  3 */ TH_COORDINATOR_PROXY,
  /*  4 */ TH_GENERAL,
  /*  5 */ TH_LISTER,
  /*  6 */ TH_MONITOR,
  /*  7 */ TH_QUERY,
  /*  8 */ TH_PARTICIPANT,
  /*  9 */ TH_PARTICIPANT_PROXY,
  /* 10 */ TH_RECLAIM,
  /* 11 */ TH_REQUEST,
  /* 12 */ TH_SCANOP,
  /* 13 */ TH_SINKOP,
  /* 14 */ TH_WRITEOP,
  /* 15 */ TH_ENUMS
  };
  
  static Str* aAM[]
  {
  /*  0 */ "none"_Str,
  /*  1 */ "ANSWER"_Str,
  /*  2 */ "COORDINATOR"_Str,
  /*  3 */ "COORDINATOR_PROXY"_Str,
  /*  4 */ "GENERAL"_Str,
  /*  5 */ "LISTER"_Str,
  /*  6 */ "MONITOR"_Str,
  /*  7 */ "QUERY"_Str,
  /*  8 */ "PARTICIPANT"_Str,
  /*  9 */ "PARTICIPANT_PROXY"_Str,
  /* 10 */ "RECLAIM"_Str,
  /* 11 */ "REQUEST"_Str,
  /* 12 */ "SCANOP"_Str,
  /* 13 */ "SINKOP"_Str,
  /* 14 */ "WRITEOP"_Str,
  /* 15 */ "enums"_Str
  };
  
  class Thread : public Object,
                 public ListNode_t<Thread>
  {
  private:

  public:
    i8           m_iThreadNo;    // our thread no
    s32          m_sName;        // thread name
    eTH          m_eThread;      // thread kind 
    PosixThread* m_pPosixThread; // its posix structure
    Semaphore*   m_pSemaphore;   // this threads semaphore
    TaskSet*     m_pTaskSet;     // task set this thread serves
    NP_Task      m_npTask;       // task current being executed
                                 //   note that while task is being exected
                                 //   its not in task set
    Schedulers::Scheduler*  m_pScheduler; // scheduler
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Thread* pThread);
    void  operator delete(void*);
    
    Thread(void);
    ~Thread(void);
    
    static void* vEntry_Drone(void* pArg);
    static void* vEntry_General(void* pArg);
    static void* vEntry_Partition(void* pArg);
    static void* vEntry_Worker(void* pArg);
    static Thread* pThread(void);
    static void vRunTask(MP_Task mpTask);
  
    void vAppend(MP_Task mpTask);
    void vBind(i8 iCpu);
    i8   iCpu(void);
    void vContentDump(void) const;
    void vLoop_Drone(void);
    void vLoop_General(void);
    void vLoop_Partition(void);
    void vLoop_Worker(void);
    StrVal&   sName(void);
    void vName(StrVal& sName);
    void vSpawn_Drone(void);
    void vSpawn_General(void);
    void vSpawn_Partition(void);
  void vSpawn_Worker(void);
    i8   iThreadNo(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    void  vCondBroadcast(void);
    void  vCondSignal(void);
    void  vCondWait(void);
    void  vCondTimedWait(i8 iNanoSeconds);
    bool  bMutexLock(void);
    void  vMutexLock(void);
    void  vMutexUnlock(void);
    void vSignalThread(void);
  
    Footer* aFooter(void);
  };
  
  Thread* _pThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
