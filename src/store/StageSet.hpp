/*============================================================================*/
/* StageSet.hpp -- List of Stages                                             */
/*============================================================================*/
#pragma once

#include "Stage.hpp"

namespace STORE
{
  class StageSet : public ListNL_t<Stage>
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSet(void);
   ~StageSet(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

StageSet* _pStageSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
