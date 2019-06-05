/*============================================================================*/
/* snDTE.hpp - Dte scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snDTE : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snDTE(void);
    ~snDTE(void) noexcept(false);
  
    i8 i(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
