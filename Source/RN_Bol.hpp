/*============================================================================*/
/* RN_Bol.hpp - Bol random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Bol : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Bol(void);
   ~RN_Bol(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
