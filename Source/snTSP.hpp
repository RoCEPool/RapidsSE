/*============================================================================*/
/* snTSP.hpp - Tme scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snTSP : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snTSP(void);
    ~snTSP(void) noexcept(false);
  
    Stz* sz(i8 iTme);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
