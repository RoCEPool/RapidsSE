/*============================================================================*/
/* RN_Bcd.hpp - Bcd random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Bcd : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Bcd(void);
   ~RN_Bcd(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
