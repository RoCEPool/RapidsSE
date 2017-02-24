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
    i8          m_iThreadNo;    // our thread no
    s32         m_sName;        // thread name
    PosixThread m_xPosixThread; // its posix structure
    TaskSet*    m_pTaskSet;     // task set this thread serves
    NP_Task     m_npTask;       // task current being executed
                                //   note that while task is being exected
                                //   its not in task set
    
    // must be last
    Footer m_xFooter;
    
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
    void vContentDump(void) const;
    void vLoop(void);
    sRefactor&   sName(void);
    void vName(sRefactor& sName);
    void vSpawn(void);
    i8   iThreadNo(void);
    
    Footer* pFooter(void);
  };
  
  Thread* _pThread(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
