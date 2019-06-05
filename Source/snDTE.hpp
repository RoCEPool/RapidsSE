/*============================================================================*/
/* snDTE.hpp - Dte sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snDTE : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snDTE(void);
    ~snDTE(void) noexcept(false);
  
    Stz* sz(i8 iDte);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
