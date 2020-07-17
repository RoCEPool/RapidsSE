/*============================================================================*/
/* SP_Ivl.hpp - Ivl sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class SP_Ivl : public SPsmall
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Ivl(void);
    ~SP_Ivl(void) noexcept(false);
  
    Stz* sz(i8 iIvl);
    Stz* sz(i8 iIvl, i8 iScale);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
   
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
