/*============================================================================*/
/* TaskList.hpp - list of tasks                                               */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Handlers;
}

namespace Tasks
{
  class TaskList : public ListNL_t<Task>
  {
  private:
    Lock m_oLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskList* pTaskQueue);
    void  operator delete(void*);
    
    TaskList(void);
    ~TaskList(void) noexcept(false);
    
    void   vAddTask(Task* pTask);
    void   vDropLock(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bIsLocked(void);
    Task*  pPeekTask(void);
    Task*  pPopTask(void);
    void   vRemoveTask(Task* pTask);
    i8     nTasks(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  TaskList* _pTaskList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
