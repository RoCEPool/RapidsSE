/*============================================================================*/
/* TaskQueue.hpp - queue of tasks                                             */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Handlers;
}

namespace Tasks
{
  class TaskQueue : public Object
  {
  private:
    Lock      m_oLock;
    SkipList* m_pTaskSkipList;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskQueue* pTaskQueue);
    void  operator delete(void*);
    
    TaskQueue(cmpFun fCompare);
    ~TaskQueue(void) noexcept(false);
    
    void   vAddTask(Task* pTask);
    void   vDropLock(void);
    void  vContentDump(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bIsLocked(void);
    Task*  pPeekTask(void);
    Task*  pPopTask(void);
    void   vRemoveTask(Task* pTask);
    i8     nTasks(void);
  
    Footer* aFooter(void);
  };
  
  TaskQueue* _pTaskQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
