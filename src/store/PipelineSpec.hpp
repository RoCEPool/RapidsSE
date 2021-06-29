/*============================================================================*/
/* PipelineSpec.hpp -- pipeline Specification                                 */
/*============================================================================*/
#pragma once

namespace STORE { class StageSpec; class OpSpec; }

namespace STORE
{
  class PipelineSpec : public VectorNL_t<CN_PIPELINESPEC,StageSpec,C_MAX_STAGES>
  {
  private:
    s32  m_sPipelineNm; // pipeline name
  
    Job* m_pPipelineJob; // which job
  
    //     m_pNestedPipelines[]
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipelineSpec(Stz* szPipelineNm, Job* pJob);
   ~PipelineSpec(void) noexcept(false);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    StageSpec* pFindStageSpec(Stz* szFindNm);
    Job* pPipelineJob(void);
    Stz* szPipelineNm(void);
    StageSpec* pStageSpec(i8 i_Stage);

    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
