/*============================================================================*/
/* spDTE.hpp - Dte sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
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
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
