/*============================================================================*/
/* TaskQueue.hpp - queue of tasks                                             */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class TaskQueue : public Object
  {
  private:
    eQK         m_eKind;
    Lock        m_okLock;
    SkipListNL* m_pTaskSkipList; // skip list of tasks (ordered by (wakeup time))
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskQueue* pTaskQueue);
    void  operator delete(void*);
    
    TaskQueue(eQK eKind, bool bUniqueKey, CompareFp fpCompare);
    ~TaskQueue(void) noexcept(false);
    
    void  vAddTask(Task* pTask);
    void  vAssertItems(i8 nItems);
    void  vDropLock(void);
    void  vContentDump(void);
    void  vGrabLock(void);
    bool  bGrabLock(void);
    bool  bIsLocked(void);
    Task* pPeekTask(void);
    Task* pPopTask(void);
    void  vPortrait(s48K& sPortrait);
    void  vRemoveTask(Task* pTask);
    i8    nTasks(void);
  
    Footer* aFooter(void);
  };
  
  TaskQueue* _pTaskQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
