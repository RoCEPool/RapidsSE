/*============================================================================*/
/* LrwBuffer.hpp -- Lrw Buffer                                                */
/*============================================================================*/
#pragma once

static const i8 C_WSP_LRWBUFFER = C_SIZE_LARGE_OBJECT - C_4KB;

namespace CompactBuffers
{
  class LrwBuffer : public DataBuffer
  {
  private:
    i8 m_nItems;
  
    Wsp_t<C_WSP_LRWBUFFER> m_xWsp;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LrwBuffer(void);
   ~LrwBuffer(void) noexcept(false);
  
    Lrw* pAllotLrw(i8 nBytes);
  
    Lrw* pEach(Lrw* pLrwPrev);
  
  
  private:
    Footer* aFooter(void);
  };
  
  LrwBuffer* _pLrwBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
