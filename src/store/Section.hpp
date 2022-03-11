/*============================================================================*/
/* Section.hpp -- Section of Pipeline (being a vector of Stage)               */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"

namespace STORE { class Stage; class SectionSpec; }

namespace STORE
{
  class Section : public VectorNL_t<CN_STAGE,Stage,C_MAX_STAGES>,
                  public LifeGuard_t<Section>
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sSectionNm;  // name of this stage
    i8   m_i_Section;   // [section] in pipeline

    SectionSpec* m_pSectionSpec; // specification of this section
    Pipeline*    m_pPipeline;    // which pipeline
  
    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Section(SectionSpec* pSectionSpec, Pipeline* pPipeline);
   ~Section(void) noexcept(false);
  

      
  private:

    Footer* aFooter(void);
  };
  
  Section* _pSection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
