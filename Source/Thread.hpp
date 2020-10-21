/*============================================================================*/
/* Thread.hpp -- thread                                                       */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Thread : public Focal,
                 public ListItem_t<Thread>
  {
  public:  
    eHS      m_eThreadState;  // thread state
    eCN      m_eDedicated;    // thread is dedicated to one specific job?
    Mutex*   m_pMutex;        // this threads mutex

    Task*    m_pTask;         // task current being executed
    i8       m_nAssignedTasks;// no of tasks scheduled so far to this thread

    PsxThd*  m_pPsxThd;       // its posix structure
  
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
    void vContentDump(void);
    void vFocalOutline(s4K& sOutline);
    void vLoop_dedicated(void);
    void vLoop_general(void);
    void vSpawnPsxThd(void* pFun(void*), void* pArg);
    void vSpawnPsxThd_dedicated(void);
    void vSpawnPsxThd_general(void);
    void vSpawnPsxThd_dummy(void);
    Task* pTask(void);
    void vThisTLSThread(void);
    void vThreadEntry(void);
    i8   iThreadNo(void);
    eHS  eThreadState(void);
  
  private:

    Footer* aFooter(void);
  };
  
  Thread* _pThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
