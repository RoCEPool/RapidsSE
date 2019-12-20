/*============================================================================*/
/* RN_Flt.hpp - Flt random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Flt : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Flt(void);
   ~RN_Flt(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
