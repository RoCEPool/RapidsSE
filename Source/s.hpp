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
    void* operator new(size_t);
    
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
