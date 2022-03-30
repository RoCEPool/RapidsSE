/*============================================================================*/
/* StageSpecSet.hpp -- List of Stage Specifications                           */
/*============================================================================*/
#pragma once

#include "StageSpec.hpp"

namespace STORE
{
  class StageSpecSet : public ListNL_t<StageSpec>
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpecSet(void);
   ~StageSpecSet(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

StageSpecSet* _pStageSpecSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
