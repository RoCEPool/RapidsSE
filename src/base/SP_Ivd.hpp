/*============================================================================*/
/* SP_Ivd.hpp - Ivd sprint helper                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_Ivd : public SPsmall
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Ivd(void);
    ~SP_Ivd(void) noexcept(false);
  
    Stz* sz(i8 iIvd);
    Stz* sz(i8 iIvd, i8 iScale);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
   
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
