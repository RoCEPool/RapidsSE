/*============================================================================*/
/* Thread.hpp -- thread                                                       */
/*============================================================================*/
#pragma once


namespace Threads
{
  enum eTC // thread condition/state
  {
    /*  0 */ TC_NONE=0,
    /*  1 */ TC_IDLE,
    /*  2 */ TC_RUNNING,
    /*  3 */ TC_ENUMS,
  };
  
  static Stz* aTC[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "IDLE"_Stz,
    /*  2 */ "RUNNING"_Stz,
    /*  3 */ "ENUMS"_Stz,
  };
  
  class Thread : public Object
  {
  private:

  public:
    i8           m_iThreadNo;    // our thread no
    void*        m_aStackBase;   // stack base address
    i8           m_iStackSize;   // stack size
  
    eTC          m_eThreadState; // thread state
    Task*        m_pTask;        // task current being executed
    eCN          m_eDedicated;   // thread is dedicated to one specific job?

    PosixThread* m_pPosixThread; // its posix structure
    Mutex*       m_pMutex;       // this threads mutex
  
    i8           m_iLastQueueRR;  // last round robin queue this thread
                                  // had popped a task from
    i8           m_iStartQueueRR; // last RR queue we started with
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Thread* pThread);
    void  operator delete(void*);
    
    Thread(void);
    ~Thread(void) noexcept(false);
  
    static void* aEntryPoint(void* pArg);
    static void* aEntryPointDummy(void* pArg);

    static Thread* pThisTLSThread(void);
  
    void vBind(i8 iCore);
    void vBindCore(void);
    i8   iCpu(void);
    void vContentDump(void) const;
    void vExecuteTask(Task* pTask);
    void vLoop(void);
    void vReExecuteTask(Task* pTask);
    bool bRRnext(void);
    void vRRstart(void);
    void vSpawnPthread(void);
    void vSpawnPthread_Dummy(void);
    i8   iThreadNo(void);
    eTC  eThreadState(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
 
    bool  bQueuePeek(void);

    void  vTaskCall(void);
  
    bool  bGrabIfIdle(void);
  
  private:

    Footer* aFooter(void);
  };
  
  Thread* _pThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
