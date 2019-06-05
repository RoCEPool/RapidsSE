/*============================================================================*/
/* snVCH.hpp - Vch scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snVCH : public Scan
  {
  public:
  
    char m_caVch[C_MAX_VCH];
  
    // must be last
    Footer m_xFooter;
    
  public:
    snVCH(void);
    ~snVCH(void) noexcept(false);
    
    Stz* sz(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
