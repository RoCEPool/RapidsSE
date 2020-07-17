/*============================================================================*/
/* SP_Flt.hpp - Flt sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class SP_Flt : public SPsmall
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Flt(void);
    ~SP_Flt(void) noexcept(false);
  
    Stz* sz(d8 dFlt);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
