/*============================================================================*/
/* Value.hpp -- Base Class for a Self-Describing Value                        */
/*============================================================================*/
#pragma once

namespace Tasks {
  class Task;
  class TaskVal;
  typedef MP<Task,CN_TASK> MP_Task;
}

namespace Forth {
  class Cell;
  class ForthWord;
  class WrdVal;
}

namespace Values
{
  class IntVal;
  class FipVal;
  class PtrVal;
  class StrVal;
  
  class Value
  {
  protected:
    Header m_xHeader;
  
  public:
    void* operator new(size_t);
    Value(eCN eClass);
  
    eCN  eClass(void);
    i8   nSize(void);
    void vToString(Stz* aString);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
