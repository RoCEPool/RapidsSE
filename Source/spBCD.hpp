/*============================================================================*/
/* spBCD.hpp - Bcd sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spBCD : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spBCD(void);
    ~spBCD(void) noexcept(false);
  
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
