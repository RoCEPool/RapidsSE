/*============================================================================*/
/* RsbBuffer.hpp -- RowSlab Buffer                                            */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class RsbBuffer : public DataBuffer
  {
  private:
  
    RowSlab m_oRowSlab;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RsbBuffer(void);
   ~RsbBuffer(void) noexcept(false);
        
  private:
    Footer* aFooter(void);
  };
  
  RsbBuffer* _pRsbBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
