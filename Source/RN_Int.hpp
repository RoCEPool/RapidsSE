/*============================================================================*/
/* RN_Int.hpp - Int random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Int : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Int(void);
   ~RN_Int(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
