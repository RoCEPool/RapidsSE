/*============================================================================*/
/* SP_Bcd.hpp - Bcd sprint helper                                             */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SP_Bcd : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Bcd(void);
    ~SP_Bcd(void) noexcept(false);
  
    Stz* sz(i8 iBcd);
    Stz* sz(i8 iBcd, i8 iScale);
  
    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
