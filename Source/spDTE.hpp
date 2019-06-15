/*============================================================================*/
/* spDTE.hpp - Dte sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class spDTE : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    spDTE(void);
    ~spDTE(void) noexcept(false);
  
    Stz* sz(i8 iDte);
  
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
