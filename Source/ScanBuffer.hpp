/*============================================================================*/
/* ScanBuffer.hpp -- Scanning Buffer                                          */
/*============================================================================*/

#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  class ScanBuffer : public Buffer
  {
  private:
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ScanBuffer(void);
    ~ScanBuffer(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
