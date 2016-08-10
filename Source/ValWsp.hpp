/*============================================================================*/
/* ValWsp.hpp - workspace for moving values to/from thread and task stacks    */
/*============================================================================*/

#pragma once

#include "StackValues.h"

namespace StackValues
{
  class ValWsp : public OnStack
  {
  private:
    char m_mArray[C_1KB*16 - sizeof(OnStack)];
    
  public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(void* pPtr);
    void operator =(char* pStr);
    void operator =(const char* pStr);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/