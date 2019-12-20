/*============================================================================*/
/* RN_Ivl.hpp - Ivl random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Ivl : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Ivl(void);
   ~RN_Ivl(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
