/*============================================================================*/
/* SectionSpec.hpp -- Section Specification                                   */
/*============================================================================*/
#pragma once

#include "eAL.h"
#include "eYG.h"
#include "eYX.h"
#include "eYF.h"

#include "StageSpec.hpp"
#include "LifeGuard_t.hpp"

namespace STORE
{
  class SectionSpec
          : public ListNL_t<StageSpec>
          , public LifeGuard_t<SectionSpec>
          , public ListItem_t<SectionSpec>

  {
  private:
    s32  m_sSectionNm; // section name

    PipelineSpec* m_pPipelineSpec; // which pipeline spec
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SectionSpec(Stz* szSectionNm, PipelineSpec* pPipelineSpec);
   ~SectionSpec(void) noexcept(false);
     
    Footer* aFooter(void);
};

  SectionSpec* _pSectionSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
