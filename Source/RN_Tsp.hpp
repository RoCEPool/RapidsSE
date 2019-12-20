/*============================================================================*/
/* RN_Tsp.hpp - Tsp random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Tsp : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Tsp(void);
   ~RN_Tsp(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
