/*============================================================================*/
/* snTME.hpp - Tme scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snTME : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snTME(void);
    ~snTME(void) noexcept(false);
  
    i8 i(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
