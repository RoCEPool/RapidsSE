/*============================================================================*/
/* spIVL.hpp - Ivl sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class spIVL : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spIVL(void);
    ~spIVL(void) noexcept(false);
  
    Stz* sz(i8 iIvl);
    Stz* sz(i8 iIvl, i8 iScale);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
