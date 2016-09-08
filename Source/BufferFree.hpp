/*============================================================================*/
/*  BufferFree.hpp - a buffer when its in the buffer pool free chain          */
/*============================================================================*/

#pragma once

#include "Pool.h"

using namespace Common;

namespace Pool
{
  class BufferFree : public Buffer
  {
  private:
    bn m_nNextFree;  // next buffer in free chain
    // ..data..
    
  public:
    BufferFree(void);
    ~BufferFree(void);
    
    bn   nNextFree(void);
    void vNextFree(bn nBuffer);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
