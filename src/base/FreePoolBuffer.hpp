/*============================================================================*/
/* FreePoolBuffer.hpp - a buffer when its in the buffer pool free chain           */
/*============================================================================*/
#pragma once

namespace BASE
{
  class FreePoolBuffer : public PoolBuffer
  {
  private:
    i8 m_iNextFree;  // next buffer in free chain
    
    
  public:
    FreePoolBuffer(void);
    ~FreePoolBuffer(void) noexcept(false);
    
    i8   iNextFree(void);
    void vNextFree(i8 iNextFree);
    
    void vZeroData(void);
    void vSetFooter(void);
    
    PoolBuffer* _pFreePoolBuffer(void* aAddr);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
