/*============================================================================*/
/* ToHshCrw.hpp -- Convert ptn nst rsb buffers to hashed crw buffers          */
/*============================================================================*/
#pragma once

namespace Load
{
  class ToHshCrw : public Object
  {
  private:
  
    HashSet    m_oHashSet;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToHshCrw(void);
   ~ToHshCrw(void) noexcept(false);
  
    void vConvert(RsbBuffer* pRsbBuffer);
  
    RsbBuffer* pCompleted(i8 i_Ptn);
    RsbBuffer* pDangling(i8 i_Ptn);
  
  private:

    Footer* aFooter(void);
  };

  ToHshCrw* _pToHshCrw(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
