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
  
  static Stz* aTH[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ANSWER"_Stz,
  /*  2 */ "COORDINATOR"_Stz,
  /*  3 */ "COORDINATOR_PROXY"_Stz,
  /*  4 */ "GENERAL"_Stz,
  /*  5 */ "LISTER"_Stz,
  /*  6 */ "MONITOR"_Stz,
  /*  7 */ "QUERY"_Stz,
  /*  8 */ "PARTICIPANT"_Stz,
  /*  9 */ "PARTICIPANT_PROXY"_Stz,
  /* 10 */ "RECLAIM"_Stz,
  /* 11 */ "REQUEST"_Stz,
  /* 12 */ "SCANOP"_Stz,
  /* 13 */ "SINKOP"_Stz,
  /* 14 */ "WRITEOP"_Stz,
  /* 15 */ "ENUMS"_Stz
  };
  
  enum eTC // thread condition/state
  {
    /*  0 */ TC_NONE=0,
    /*  1 */ TC_READYTORUN,
    /*  2 */ TC_RUNNING,
    /*  3 */ TC_WAITING,
    /*  4 */ TC_ENUMS,
  };
  
  static Stz* aTC[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "READYTORUN"_Stz,
    /*  2 */ "RUNNING"_Stz,
    /*  3 */ "WAITING"_Stz,
    /*  4 */ "ENUMS"_Stz,
  };
  
  class Thread : public Object,
                 public ListNode_t<Thread>
  {
  private:

  public:
    i8           m_iThreadNo;    // our thread no
    s32          m_sName;        // thread name
    Lock         m_xLock;        // thread lock
    eAC          m_eActorModel;  // actor model
    eTH          m_eThread;      // thread kind
    eTC          m_eThreadState; // thread state
    i8           m_iPartition;   // specific partition? 
    PosixThread* m_pPosixThread; // its posix structure
    Semaphore*   m_pSemaphore;   // this threads semaphore
    TaskFifo*    m_pTaskFifo;    // this threads task fifo/list
    TaskSet*     m_pTaskSet;     // task set this thread serves
    NP_Task      m_npTask;       // task current being executed
                                 //   note that while task is being exected
                                 //   its not in task set
    Schedulers::Scheduler*  m_pScheduler; // scheduler
  
    i8           m_iLastRRqueue; // last round robin queue this thread
                                 // had popped a task
  
    i8           m_iStartRRqueue;// last RR queue we started with

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Thread* pThread);
    void  operator delete(void*);
    
    Thread(void);
    ~Thread(void);
    
    static void* vEntry_Drone(void* pArg);
    static void* vEntry_Partition(void* pArg);
    static void* vEntry_Server(void* pArg);
    static void* vEntry_Worker(void* pArg);
  
    static void* pEntryPoint(void* pArg);
  
    static Thread* pThread(void);
    static void vRunTask(MP_Task mpTask);
  
    void vAppend(MP_Task mpTask);
    void vBind(i8 iCpu);
    i8   iCpu(void);
    void vContentDump(void) const;
    bool bGrabLock(void);
    void vDropLock(void);
    bool bIsIdle(void);
    i8   iLastRRqueue(void);
    void vLastRRqueue(i8 iLastRRqueue);
    void vLoop(void);
    void vLoop_Drone(void);
    void vLoop_Partition(void);
    void vLoop_Partition2(void);
    void vLoop_Server(void);
    void vLoop_Worker(void);
    i8   iPartition(void);
    void vPartition(i8 iPartition);
    StrVal&   sName(void);
    void vName(StrVal& sName);
    bool bRRnext(void);
    void vRRstart(void);
    void vSpawn_Drone(void);
    void vSpawn_Partition(void);
    void vSpawn_Server(void);
    void vSpawn_Thread(void);
    void vSpawn_Worker(void);
    void vTask(Task* pTask);
    i8   iThreadNo(void);
    eTC  eThreadState(void);
    void vThreadState(eTC eThreadState);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    void  vCondBroadcast(void);
    void  vCondSignal(void);
    void  vCondWait(void);
    void  vCondTimedWait(i8 iNanoSeconds);
    bool  bMutexLock(void);
    void  vMutexLock(void);
    void  vMutexUnlock(void);
    void  vSignalThread(void);
  
    bool  bQueuePeek(void);

    void  vTaskCall(void);
    TaskFifo* pTaskFifo(void);
    void  vTaskFifo(TaskFifo* pTaskFifo);
    void  vTaskWait(void);
  
    Footer* aFooter(void);
  };
  
  Thread* _pThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
