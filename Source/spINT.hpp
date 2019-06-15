/*============================================================================*/
/* spINT.hpp - Int sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spINT : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spINT(void);
    ~spINT(void) noexcept(false);
  
    Stz* sz(i8 iInt);

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
