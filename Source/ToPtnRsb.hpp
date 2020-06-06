/*============================================================================*/
/* ToPtnRsb.hpp -- Convert rsb buffers to partitioned row slab buffers        */
/*============================================================================*/
#pragma once

namespace Load
{
  class ToPtnRsb : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToPtnRsb(void);
   ~ToPtnRsb(void) noexcept(false);
  
    void vConvert(RsbBuffer* pRsbBuffer);
  
    RsbBuffer* pDangling(i8 i_Ptn);
    RsbBuffer* pFull(i8 i_Ptn);
  
  private:
    Footer* aFooter(void);
  };

  ToPtnRsb* _pToPtnRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
