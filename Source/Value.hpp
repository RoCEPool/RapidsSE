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
  
  class Value : public Header
  {
  public:
    void* operator new(size_t);
    
    Value(eCN eClass);
    
    void operator =(i8 iVal);
    
    char* aClass(void);
    i8    iSize(void);
    
    i8    iAsInt(void);
    i8    iInt(void);
    Dictionaries::Cell* pIpPtr(void);
    Object* pPtr(void);
    char* sStr(void);
    Object* pObject(void);
    Dictionaries::Word* pWord(void);
    
    BinVal* pAsBinVal(void);
    IntVal* pAsIntVal(void);
    IpVal*  pAsIpVal(void);
    PtrVal* pAsPtrVal(void);
    StrVal* pAsStrVal(void);
    ValSet* pAsValSet(void);
    
    void  vToCommand(char* aWorkspace);
    void  vToString(char* aWorkspace);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
