/*============================================================================*/
/* Formats.hpp -- Various configurable formats                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Formats : public Object
  {
  private:
    s32 m_sBcd;
    s32 m_sDte;
    s32 m_sFlt;
    s32 m_sInt;
    s32 m_sIvl;
    s32 m_sTme;
    s32 m_sTsp;
    s32 m_sVch;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pAddr);
  
    Formats(void);
   ~Formats(void) noexcept(false);

    Footer* aFooter(void);
  };
  
  Formats* _pFormats(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
