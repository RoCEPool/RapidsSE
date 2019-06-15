/*============================================================================*/
/* spFLT.hpp - Flt sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spFLT : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spFLT(void);
    ~spFLT(void) noexcept(false);
  
    Stz* sz(d8 dFlt);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
    Stz* szNull(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
