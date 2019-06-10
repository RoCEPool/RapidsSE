/*============================================================================*/
/* spVCH.hpp - Vch sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spVCH : public Sprint
  {
  public:
  
    char m_caVch[C_MAX_VCH];
  
    // must be last
    Footer m_xFooter;
    
  public:
    spVCH(void);
    ~spVCH(void) noexcept(false);
  
    Stz* sz(char* azVch);
    Stz* sz(Stz*  szVch);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
