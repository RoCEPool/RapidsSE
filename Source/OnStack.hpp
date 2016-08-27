/*============================================================================*/
/* OnStack.hpp - root class for objects on thread stack                       */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Tasks {
  class Task;
  class TaskVal;
  typedef NNP<Task,CN_TASK> NNP_Task;
}

namespace Dictionaries {
  class Word;
  class WrdVal;
}

namespace Values
{
  class IntVal;
  class PtrVal;
  class StrVal;
  class ValSet;
  class ValWsp;
  
  class OnStack : public mn,
                  public cn
  {
  public:
    void* operator new(size_t);
    
    OnStack(cn eClass);
    
    void operator =(i8 iVal);
    
    char* aClass(void);
    i8    iSize(void);
    
    i8    iAsInt(void);
    i8    iInt(void);
    void* pPtr(void);
    char* sStr(void);
    Tasks::NNP_Task nnpTask(void);
    Dictionaries::Word* pWord(void);
    
    IntVal* pAsIntVal(void);
    PtrVal* pAsPtrVal(void);
    StrVal* pAsStrVal(void);
    ValSet* pAsValSet(void);
    Dictionaries::WrdVal* pAsWrdVal(void);
    
    void  vToCommand(char* aWorkspace);
    void  vToString(char* aWorkspace);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/