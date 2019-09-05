/*============================================================================*/
/* DynamicSpec.hpp -- Dynamic Specification                                   */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class DynamicSpec : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    DynamicSpec(void);
   ~DynamicSpec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
