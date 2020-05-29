/*============================================================================*/
/* StageVec.hpp -- Stage Vector                                               */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class StageVec : public VectorNL_t<CN_STAGEVEC,Stage,C_MAX_STAGES>
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    StageVec(void);
   ~StageVec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
