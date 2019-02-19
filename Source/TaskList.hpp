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
   eQK  m_eKind;
   Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskList* pTaskQueue);
    void  operator delete(void*);
    
    TaskList(eQK eKind);
    ~TaskList(void) noexcept(false);
    
    void   vAddTask(Task* pTask);
    void   vDropLock(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bHasJobClass(eCN eJobClass);
    bool   bIsLocked(void);
    Task*  pPeekTask(void);
    Task*  pPopTask(void);
    void   vPortrait(s48K& sPortrait);
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
