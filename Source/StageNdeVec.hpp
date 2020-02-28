/*============================================================================*/
/* StageNdeVec.hpp -- Stage Partition Vector                                  */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class StageNdeVec : public VectorNL_t<CN_STAGENDEVEC,Division,C_MAX_STAGES>
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageNdeVec(void);
   ~StageNdeVec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
