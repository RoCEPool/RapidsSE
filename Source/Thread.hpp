/*============================================================================*/
/* Thread.hpp -- thread                                                       */
/*============================================================================*/
#pragma once

namespace Threads
{
  enum eTD // thread state
  {
    /*  0 */ TD_NONE=0,
    /*  1 */ TD_ASSIGNED,
    /*  2 */ TD_IDLE,
    /*  3 */ TD_ENUMS,
  };
  
  static Stz* aTD[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "ASSIGNED"_Stz,
    /*  2 */ "IDLE"_Stz,
    /*  3 */ "ENUMS"_Stz,
  };
  
  class Thread : public Object,
                 public ListNode_t<Thread>
  {
  private:

  public:
    i8       m_iThreadNo;     // our thread no
    void*    m_aStackBase;    // stack base address
    i8       m_iStackSize;    // stack size
  
    eTD      m_eThreadState;  // thread state
    Task*    m_pTask;         // task current being executed
    eCN      m_eDedicated;    // thread is dedicated to one specific job?

    Pthread* m_pPthread;      // its posix structure
    Mutex*   m_pMutexThread;  // this threads mutex
  
    i8       m_nAssignedTasks;// number of tasks scheduled to this thread
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Thread* pThread);
    void  operator delete(void*);
    
    Thread(void);
    ~Thread(void) noexcept(false);
  
    static Thread* pThisTLSThread(void);
  
    static void* aEntryPoint_dedicated(void* pArg);
    static void* aEntryPoint_dummy(void* pArg);
    static void* aEntryPoint_general(void* pArg);

    void vBind(i8 iCore);
    void vBindCore(void);
    i8   iCpu(void);
    void vContentDump(void) const;
    void vExecuteTask(Task* pTask);
    void vLoop_dedicated(void);
    void vLoop_general(void);
    void vReExecuteTask(Task* pTask);
    void vSpawnPthread_dedicated(void);
    void vSpawnPthread_general(void);
    void vSpawnPthread_dummy(void);
    Task* pTask(void);
    void vThisTLSThread(void);
    void vThreadEntry(void);
    i8   iThreadNo(void);
    eTD  eThreadState(void);
  
  private:

    Footer* aFooter(void);
  };
  
  Thread* _pThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
