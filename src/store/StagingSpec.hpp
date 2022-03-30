/*============================================================================*/
/* StageSpecSet.hpp -- List of Stage Specifications                           */
/*============================================================================*/
#pragma once

#include "StageSpec.hpp"

namespace STORE
{
  class StagingSpec : public ListNL_t<StageSpec>
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StagingSpec(void);
   ~StagingSpec(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

StagingSpec* _pStagingSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
