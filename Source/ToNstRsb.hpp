/*============================================================================*/
/* ToNstRsb.hpp -- Convert rsb to normalised string row slab buffers          */
/*============================================================================*/
#pragma once

namespace Load
{
  class ToNstRsb : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToNstRsb(void);
   ~ToNstRsb(void) noexcept(false);
  
    void vConvert(RsbBuffer* pRsbBuffer);
  
  private:
    Footer* aFooter(void);
  };

  ToNstRsb* _pToNstRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
