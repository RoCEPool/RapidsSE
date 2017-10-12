/*============================================================================*/
/* TaskSet.hpp - set of tasks                                                 */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class TaskSet : public Object,
                  public List_t<Task>
  {
  private:
    Lock    m_xLock;
    i8      m_iCount;
    NP_Task m_npHead;
    NP_Task m_npTail;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskSet* pTaskSet);
    void  operator delete(void*);
    
    TaskSet(void);
    ~TaskSet(void);
    
    void  vAppend(MP_Task mpTask);
    void  vContentDump(void);
    NP_Task npPop(void);
    void  vRemove(MP_Task mpTask);
    
  private:
    void vVerify(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
