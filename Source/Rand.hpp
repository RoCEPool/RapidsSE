/*============================================================================*/
/* Rand.hpp - Rand Gadget for generating random datums                        */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class Rand : public Object
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    Rand(eCN eClass);
   ~Rand(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
