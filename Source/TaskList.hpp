/*============================================================================*/
/* TaskList.hpp - Thread Set                                                  */
/*============================================================================*/

#pragma once

#include "Tasks.h"

namespace Tasks {
    
  class TaskList : public Object,
                   public List_t<Task>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TaskList(void);
    ~TaskList(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Task* pAllocTask(void);
    void  vFreeTask(Task* pTask);
    void vDump(void);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/