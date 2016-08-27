/*============================================================================*/
/* ValWsp.hpp - workspace for moving values to/from thread and task stacks    */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class ValWsp : public Value
  {
  private:
    char m_mArray[C_1KB*16 - sizeof(Value)];
    
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