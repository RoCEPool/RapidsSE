/*============================================================================*/
/* RN_Vch.hpp - Vch random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Vch : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Vch(void);
   ~RN_Vch(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
