/*============================================================================*/
/* Value.hpp -- Base Class for a Self-Describing Value                        */
/*============================================================================*/
#pragma once

namespace Tasks {
  class Task;
  class TaskVal;
  typedef MP<Task,CN_TASK> MP_Task;
}

namespace Dictionaries {
  class Cell;
  class Word;
  class WrdVal;
}

namespace Values
{
  class BinVal;
  class IntVal;
  class IpVal;
  class PtrVal;
  class StrVal;
  class ValSet;
  class ValWrk;
  
  class Value : public Header
  {
  public:
    void* operator new(size_t);
    
    Value(eCN eClass);
  
    i8   nSize(void);
    void vToString(Stz* aString);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
