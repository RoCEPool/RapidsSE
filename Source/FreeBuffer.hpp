/*============================================================================*/
/* FreeBuffer.hpp - a buffer when its in the buffer pool free chain           */
/*============================================================================*/
#pragma once

namespace Pool
{
  class FreeBuffer : public PoolBuffer
  {
  private:
    bn m_nNextFree;  // next buffer in free chain
    
    
  public:
    FreeBuffer(void);
    ~FreeBuffer(void) noexcept(false);
    
    bn   nNextFree(void);
    void vNextFree(bn nBuffer);
    
    void vZeroData(void);
    void vSetFooter(void);
    
    PoolBuffer* _pFreeBuffer(void* aAddr);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
