/*============================================================================*/
/* spTSP.hpp - Tme sprint helper                                              */
/*============================================================================*/
#pragma once

namespace ScanSprint
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
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
