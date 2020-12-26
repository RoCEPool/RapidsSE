/*============================================================================*/
/* FreeBuffer.hpp - a buffer when its in the buffer pool free chain           */
/*============================================================================*/
#pragma once

namespace Resources
{
  class FreeBuffer : public PoolBuffer
  {
  private:
    i8 m_iNextFree;  // next buffer in free chain
    
    
  public:
    FreeBuffer(void);
    ~FreeBuffer(void) noexcept(false);
    
    i8   iNextFree(void);
    void vNextFree(i8 iNextFree);
    
    void vZeroData(void);
    void vSetFooter(void);
    
    PoolBuffer* _pFreeBuffer(void* aAddr);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
