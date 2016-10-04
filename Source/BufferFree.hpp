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
    
    
  public:
    BufferFree(void);
    ~BufferFree(void);
    
    bn   nNextFree(void);
    void vNextFree(bn nBuffer);
    
    void vZeroData(void);
    void vSetFooter(void);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
