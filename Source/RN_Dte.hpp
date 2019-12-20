/*============================================================================*/
/* RN_Dte.hpp - Dte random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Dte : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Dte(void);
   ~RN_Dte(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
