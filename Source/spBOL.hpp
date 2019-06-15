/*============================================================================*/
/* spBOL.hpp - Bol sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spBOL : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spBOL(void);
    ~spBOL(void) noexcept(false);
  
    Stz* sz(i8 iBol);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
    Stz* szNull(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
