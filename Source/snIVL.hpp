/*============================================================================*/
/* snIVL.hpp - Ivl sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snIVL : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snIVL(void);
    ~snIVL(void) noexcept(false);
  
    Stz* sz(i8 iIvl);
    Stz* sz(i8 iIvl, i8 iScale);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
