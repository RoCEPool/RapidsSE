/*============================================================================*/
/* Thread.hpp - thread                                                        */
/*============================================================================*/

#pragma once

#include "Threading.h"

namespace Threading
{
  class Thread : public Object,
                 public ListNode_t<Thread>
  {
  private:

  public:
    
    PosixThread m_xPosixThread;
    NP_Task     m_npTask;
    
    TaskSet*    m_pTaskSet;
    
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
    void vSpawn(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
