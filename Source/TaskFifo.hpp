/*============================================================================*/
/* TaskFifo.hpp - fifo of tasks                                               */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class TaskFifo : public Object,
                   public List_t<Task>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskFifo* pTaskFifo);
    void  operator delete(void*);
    
    TaskFifo(void);
    ~TaskFifo(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  TaskFifo* _pTaskFifo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
