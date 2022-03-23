/*============================================================================*/
/* PipelineSpec.hpp -- pipeline Specification                                 */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"
#include "eYS.h"


namespace STORE { class Dynamo;
                  class PipeworkSpec; class StageSpec; class OpSpec;
                  class SubPipelines; }

namespace STORE
{
  class PipelineSpec
          : public Object
          , public LifeGuard_t<PipelineSpec>
          , public ListItem_t<PipelineSpec>
  {
    friend class STORE::Dynamo;

  private:
  
    PipeworkSpec* m_pPipeworkSpec;

    SubPipelines* m_pSubPipelines;
  
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
    Scanner* m_p_Scanner; // statement token scanner
    TagSet*  m_p_JobTags; // job tag set
 
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipelineSpec(Scanner* p_Scanner, TagSet* p_JobTags); 
   ~PipelineSpec(void) noexcept(false);
  
    bool bCompile(void);
    void vDump(void);
    TagSet* pJobTags(void);
    Stz* szPipelineNm(void);
    PipeworkSpec* pPipeworkSpec(void);
    i8 nStages(void);
    StageSpec* pStageSpec(i8 i_Stage);
    SubPipelines* pSubPipelines(void);

  private:
    Footer* aFooter(void);
  };

  PipelineSpec* _pPipelineSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
