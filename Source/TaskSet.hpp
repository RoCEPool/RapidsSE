/*============================================================================*/
/* TaskSet.hpp - set of tasks                                                 */
/*============================================================================*/

#pragma once

#include "Tasking.h"

namespace Tasking
{
  class TaskSet : public Object,
                  List_t<Task>
  {
  private:
    Lock    m_xLock;
    i8      m_iCount;
    NP_Task m_npHead;
    NP_Task m_npTail;
    
  public:
    void* operator new(size_t, TaskSet* pTaskSet);
    void  operator delete(void*);
    
    TaskSet(void);
    ~TaskSet(void);
    
    void  vAppend(NNP_Task nnpTask);
    void  vDump(void);
    NP_Task npPop(void);
    void  vRemove(NNP_Task nnpTask);
    
  private:
    void vVerify(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
