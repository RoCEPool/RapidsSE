/*============================================================================*/
/* TxVal.hpp - tx pointer value                                               */
/*============================================================================*/

#pragma once

#include "Tasking.h"

namespace Tasking
{
  class TaskVal : public OnStack
  {
  private:
    Task* m_pTask;
    
  public:
    void* operator new(size_t);
    
    TaskVal(void);
    
    void operator =(Task* pTask);
    
    i8    iSize(void) const;
    Task* pTask(void) const;

    void  vToCommand(char* aWorkspace);
    void  vToString(char* aWorkspace);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/