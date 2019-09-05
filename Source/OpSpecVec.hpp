/*============================================================================*/
/* OpSpecVec.hpp -- Op Specification Vector                                   */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class OpSpecVec : public VectorNL_t<CN_OPSPECVEC,OpSpec,C_MAX_OPS>
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    OpSpecVec(void);
   ~OpSpecVec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
