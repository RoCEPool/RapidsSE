/*============================================================================*/
/* spTSP.hpp - Tme sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spTSP : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spTSP(void);
    ~spTSP(void) noexcept(false);
  
    Stz* sz(i8 iTme);
  
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
