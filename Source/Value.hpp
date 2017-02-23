/*============================================================================*/
/* Value.hpp -- Base Class for a Self-Describing Value                        */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Tasks {
  class Task;
  class TaskVal;
  typedef NNP<Task,CN_TASK> NNP_Task;
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
  class ValAcc;
  class ValSet;
  class ValWsp;
  
  class Value : public Object
  {
  public:
    void* operator new(size_t);
    
    Value(eCN eClass);
    
    
    i8    iSize(void);
    
    i8    iAsInt(void);
    Object* pNewObjBin(void);
    Dictionaries::Word* pWord(void);
        
    void  vToCommand(char* aWorkspace);
    void  vToString(char* aWorkspace);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
