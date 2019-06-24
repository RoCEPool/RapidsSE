/*============================================================================*/
/* SP_Bol.hpp - Bol sprint helper                                             */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SP_Bol : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Bol(void);
    ~SP_Bol(void) noexcept(false);
  
    Stz* sz(i8 iBol);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
