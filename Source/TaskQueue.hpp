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
    Lock      m_xLock;
    SkipList* m_pTaskSkipList;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskQueue* pTaskQueue);
    void  operator delete(void*);
    
    TaskQueue(cmpFun fCompare);
    ~TaskQueue(void);
    
    void   vAddTask(Task* pTask);
    void   vDropWrite(void);
    void   vGrabWrite(void);
    bool   bGrabWrite(void);
    bool   bIsLocked(void);
    Task*  pPeekTask(void);
    Task*  pPopTask(void);
    void   vRemoveTask(Task* pTask);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  TaskQueue* _pTaskQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
