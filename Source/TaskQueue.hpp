/*============================================================================*/
/* TaskQueue.hpp - queue of tasks                                             */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class TaskQueue : public Object
  {
  private:
    
    SkipList* m_pSkipList;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskFifo* pTaskFifo);
    void  operator delete(void*);
    
    TaskQueue(void);
    ~TaskQueue(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  TaskQueue* _pTaskQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
