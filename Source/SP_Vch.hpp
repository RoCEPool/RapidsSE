/*============================================================================*/
/* SP_Vch.hpp - Vch sprint helper                                             */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SP_Vch : public Sprint
  {
  public:
  
    char m_caVch[C_MAX_VCH];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Vch(void);
    ~SP_Vch(void) noexcept(false);
  
    Stz* sz(char* azVch);
    Stz* sz(Stz*  szVch);
  
    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
