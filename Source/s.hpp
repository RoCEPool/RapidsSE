/*============================================================================*/
/* s.hpp -- an s-string                                                       */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  class s : public sRefactor
  {
  private:
    
  public:
    
    s(eCN eClass, i8 iSize);
   ~s(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
