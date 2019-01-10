/*============================================================================*/
/* Scratch.hpp -- scratch area                                                */
/*============================================================================*/
#pragma once

namespace System
{
  class Scratch : public Object
  {
    static const i8 C_WSP_SCRATCH = C_8KB;
  
  private:
    WspFxd<C_WSP_SCRATCH> m_xWsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Scratch* pScratch);
    void operator delete(void* aAddress);
    Scratch(void);
    ~Scratch(void) noexcept(false);

    char* aWspArea(void);
    Stz*  aWspStr(void);
    i8    nWspLength(void);
  
    Footer* aFooter(void);
  };
  
  Scratch* _pScratch(void* aAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
