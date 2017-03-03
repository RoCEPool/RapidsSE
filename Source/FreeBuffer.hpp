/*============================================================================*/
/* FreeBuffer.hpp - a buffer when its in the buffer pool free chain           */
/*============================================================================*/

#pragma once

#include "Pool.h"

using namespace System;

namespace Pool
{
  class FreeBuffer : public Buffer
  {
  private:
    bn m_nNextFree;  // next buffer in free chain
    
    
  public:
    FreeBuffer(void);
    ~FreeBuffer(void);
    
    bn   nNextFree(void);
    void vNextFree(bn nBuffer);
    
    void vZeroData(void);
    void vSetFooter(void);
    
    Buffer* _pFreeBuffer(void* aAddr);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
