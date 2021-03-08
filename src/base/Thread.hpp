/*============================================================================*/
/* Thread.hpp -- thread                                                       */
/*============================================================================*/
#pragma once

#include "eHS.h"

namespace BASE
{
  class Thread : public Loggable,
                 public ListItem_t<Thread>
  {
  public:  
    eCN      m_eDedicated;    // thread is dedicated to one specific job?
    Mutex    m_oMutex;        // this threads mutex

  private:
    eHS      m_eThreadState;  // thread state
  
    PsxThd*  m_pPsxThd;       // its posix structure
    Task*    m_pTask;         // task current being executed

    Object*  m_pObject0;      // general purpose object 0
    Object*  m_pObject1;      // general purpose object 1

    i8       m_nAssignedTasks;// no of tasks scheduled so far to this thread
  
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

    i8   nAssignedTasks(void);
    void vBind(i8 iCore);
    void vBindCore(void);
    i8   iCpu(void);
    void vContentDump(void);
    void vOutline(s4K& sOutline);
    void vLoop_dedicated(void);
    void vLoop_general(void);
    Object* pObject0(void);
    void  vObject0(Object* pObject0);
    Object* pObject1(void);
    void  vObject1(Object* pObject1);
    PsxThd* pPsxThd(void);
    void vSetIdle(void);
    void vSpawnPsxThd(void* pFun(void*), void* pArg);
    void vSpawnPsxThd_dedicated(void);
    void vSpawnPsxThd_general(void);
    void vSpawnPsxThd_dummy(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vThisTLSThread(void);
    void vThreadEntry(void);
    i8   iThreadNo(void);
    eHS  eThreadState(void);
    void vThreadState(eHS eThreadState);

  private:

    Footer* aFooter(void);
  };
  
  Thread* _pThread(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
