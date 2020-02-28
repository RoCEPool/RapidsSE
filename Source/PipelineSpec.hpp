/*============================================================================*/
/* PipelineSpec.hpp -- pipeline Specification                                 */
/*============================================================================*/
#pragma once

namespace PipelineSpecs
{
  class PipelineSpec : public VectorNL_t<CN_PIPELINESPEC,StageSpec,C_MAX_STAGES>
  {
  private:
  
    s32  m_sPipelineNm;
  
    Job* m_pJob;
  
    StageSpecVec* m_pStageSpecVec;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipelineSpec(Stz* szPipelineNm, Job* pJob);
   ~PipelineSpec(void) noexcept(false);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    StageSpec* pFindStageSpec(Stz* szFindNm, i8& i_Stage);
    Stz* szPipelineNm(void);
    StageSpec* pStageSpec(i8 i_Stage);
    StageSpecVec* pStageSpecVec(void);

    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
