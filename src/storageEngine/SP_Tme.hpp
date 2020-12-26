/*============================================================================*/
/* SP_Tme.hpp - Tme sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Arguments
{
  class SP_Tme : public SPsmall
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Tme(void);
    ~SP_Tme(void) noexcept(false);
  
    Stz* sz(i8 iTme);
  
    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
