/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class StageSpec : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    StageSpec(void);
   ~StageSpec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
