/*============================================================================*/
/* Thread.hpp -- thread                                                       */
/*============================================================================*/
#pragma once

namespace Schedulers
{
  class QueryScheduler;
}

namespace Threads
{
  
  class Thread : public Object,
                 public ListNode_t<Thread>
  {
  private:

  public:
    i8           m_iThreadNo;    // our thread no
    s32          m_sName;        // thread name
    PosixThread* m_pPosixThread; // its posix structure
    Semaphore*   m_pSemaphore;   // this threads semaphore
    TaskSet*     m_pTaskSet;     // task set this thread serves
    NP_Task      m_npTask;       // task current being executed
                                 //   note that while task is being exected
                                 //   its not in task set
    Schedulers::QueryScheduler*  m_pScheduler; // scheduler
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Thread* pThread);
    void  operator delete(void*);
    
    Thread(void);
    ~Thread(void);
    
    static void* vEntry(void* pArg);
    static void* vEntryV2(void* pArg);
    static Thread* pThread(void);
    static void vRunTask(NNP_Task nnpTask);
  
    void vAppend(NNP_Task nnpTask);
    void vBind(i8 iCpu);
    i8   iCpu(void);
    void vContentDump(void) const;
    void vLoop(void);
    void vLoopV2(void);
    StrVal&   sName(void);
    void vName(StrVal& sName);
    void vSpawn(void);
    void vSpawnV2(void);
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
  
    Footer* pFooter(void);
  };
  
  Thread* _pThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
