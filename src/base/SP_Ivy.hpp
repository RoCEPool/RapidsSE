/*============================================================================*/
/* SP_Ivy.hpp - Ivy sprint helper                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_Ivy : public SPsmall
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Ivy(void);
    ~SP_Ivy(void) noexcept(false);
  
    Stz* sz(i8 iIvy);
    Stz* sz(i8 iIvy, i8 iScale);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
   
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
