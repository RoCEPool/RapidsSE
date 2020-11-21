/*============================================================================*/
/* SP_String.hpp - String sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_String : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_String(void);
    ~SP_String(void) noexcept(false);
  
    Stz* szChar(char cChar);
    Stz* szInspect(char* azString, i8 nAfter=32, bool bQuitNL=true);
    Stz* szRight(Stz* szString, i8 iLength);
    Stz* szSample(Stz* szString, Stz* szCursor,
                  i8 iAfter=32, i8 iBefore=8);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
