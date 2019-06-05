/*============================================================================*/
/* snVCH.hpp - Vch scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snVCH : public Sprint
  {
  public:
  
    char m_caVch[C_MAX_VCH];
  
    // must be last
    Footer m_xFooter;
    
  public:
    snVCH(void);
    ~snVCH(void) noexcept(false);
  
    Stz* sz(char* azVch);
    Stz* sz(Stz*  szVch);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
