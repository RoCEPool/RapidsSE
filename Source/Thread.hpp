/*============================================================================*/
/* Thread.hpp - thread                                                        */
/*============================================================================*/

#pragma once

#include "Threads.h"

namespace Threads
{
  class Thread : public Object,
                 public ListNode_t<Thread>
  {
  private:

  public:
    s32         m_sName;        // thread name
    PosixThread m_xPosixThread; // its posix structure
    TaskSet*    m_pTaskSet;     // task set this thread serves
    NP_Task     m_npTask;       // the task being executed
    
  public:
    void* operator new(size_t, Thread* pThread);
    void  operator delete(void*);
    
    Thread(void);
    ~Thread(void);
    
    static void* vEntry(void* pArg);
    static Thread* pThread(void);
    
    void vAppend(NNP_Task nnpTask);
    void vBind(i8 iCpu);
    i8   iCpu(void);
    void vDump(void) const;
    void vLoop(void);
    s&   sName(void);
    void vName(s& sName);
    void vSpawn(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/