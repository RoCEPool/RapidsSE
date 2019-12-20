/*============================================================================*/
/* RN_Tme.hpp - Tme random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Tme : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Tme(void);
   ~RN_Tme(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
