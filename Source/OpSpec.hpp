/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class OpSpec : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    OpSpec(void);
   ~OpSpec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
