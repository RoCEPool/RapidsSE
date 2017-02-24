/*============================================================================*/
/* StrVal.hpp - string value                                                  */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  class StrVal : public sRefactor
  {
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, StrVal* pStrVal);
    
    StrVal(void);
    StrVal(i8 iChars);
    
    void operator =(const char* sStr);
    
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
