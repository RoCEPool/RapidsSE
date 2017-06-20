/*============================================================================*/
/* TaskCursor.hpp - Cursor over task list                                     */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class TaskCursor : public Object,
                     public ListCursor_t<Task>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, TaskCursor* pTaskCursor);
    void  operator delete(void*);
    
    TaskCursor(List_t<Task>* pList);
    ~TaskCursor(void);
    
    Footer* aFooter(void);
  };
  
  TaskCursor* _pTaskCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
