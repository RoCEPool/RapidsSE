/*============================================================================*/
/* snTME.hpp - Tme sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snTME : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snTME(void);
    ~snTME(void) noexcept(false);
  
    Stz* sz(i8 iTme);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
