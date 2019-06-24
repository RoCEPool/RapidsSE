/*============================================================================*/
/* SP_Dte.hpp - Dte sprint helper                                             */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SP_Dte : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Dte(void);
    ~SP_Dte(void) noexcept(false);
  
    Stz* sz(i8 iDte);
  
    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
