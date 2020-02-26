/*============================================================================*/
/* StageSpecVec.hpp -- Stage Specification Vector                             */
/*============================================================================*/
#pragma once

namespace PipelineSpecs
{
  class StageSpecVec : public VectorNL_t<CN_STAGESPECVEC,StageSpec,C_MAX_STAGES>
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpecVec(void);
   ~StageSpecVec(void) noexcept(false);
  
    Footer* aFooter(void);
  
    StageSpec* pFindStage(Stz* szStageNm);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
