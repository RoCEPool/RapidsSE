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

    PipeSpec* m_pPipeSpec; // which pipeline spec
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SectionSpec(Stz* szSectionNm, PipeSpec* pPipeSpec);
   ~SectionSpec(void) noexcept(false);
     
    Stz* szSectionNm(void);
    i8 i_SectionSpec(void);

    Footer* aFooter(void);
};

  SectionSpec* _pSectionSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
