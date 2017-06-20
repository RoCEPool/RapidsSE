/*============================================================================*/
/* TaskList.hpp - Thread Set                                                  */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class TaskList : public Object,
                   public List_t<Task>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, TaskList* pTaskList);
    void  operator delete(void*);
    
    TaskList(void);
    ~TaskList(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  TaskList* _pTaskList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
