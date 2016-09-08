/*============================================================================*/
/* ValWsp.hpp - workspace for moving values to/from thread and task stacks    */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class ValWsp : public Value
  {
    static const i8 C_VALWSP_ARRAY = C_1KB*15;
    
  private:
    char m_mArray[C_VALWSP_ARRAY];
    
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