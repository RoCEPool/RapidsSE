/*============================================================================*/
/* ForthValue.hpp -- Base Class for a Self-Describing ForthValue              */
/*============================================================================*/
#pragma once

namespace Tasks {
  class Task;
  typedef MP<Task,CN_TASK> MP_Task;
}

namespace Forth {
  class Cell;
  class ForthWord;
}

namespace Values
{
  class IntVal;
  class FipVal;
  class PtrVal;
  class StrVal;
  
  class ForthValue
  {
  protected:
    Header m_xHeader;
  
  public:
    void* operator new(size_t);
    ForthValue(eCN eClass);
  
    eCN  eClass(void);
    i8   nSize(void);
    void vToString(Stz* aString);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
