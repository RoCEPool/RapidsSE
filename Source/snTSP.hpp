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
  
    i8 i(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
