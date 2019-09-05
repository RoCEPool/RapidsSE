/*============================================================================*/
/* StageSpecVec.hpp -- Stage Specification Vector                             */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class StageSpecVec : public VectorNL_t<CN_STAGESPECVEC,StageSpec,C_MAX_STAGES>
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    StageSpecVec(void);
   ~StageSpecVec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
