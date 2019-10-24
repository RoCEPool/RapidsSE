/*============================================================================*/
/* PipelineSpec.hpp -- pipeline Specification                                 */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class PipelineSpec : public Object
  {
  private:
  
    s32 m_sName;
  
    StageSpecVec* m_pStageSpecVec;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipelineSpec(Stz* szName);
   ~PipelineSpec(void) noexcept(false);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    Stz* szName(void);

    StageSpec* pFindStageSpec(Stz* szName, i8& i_Stage);
    StageSpec* pStageSpec(i8 i_Stage);
    StageSpecVec* pStageSpecVec(void);

    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
