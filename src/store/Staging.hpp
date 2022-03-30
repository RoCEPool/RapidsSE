/*============================================================================*/
/* StageSet.hpp -- List of Stages                                             */
/*============================================================================*/
#pragma once

#include "Stage.hpp"

namespace STORE
{
  class Staging : public ListNL_t<Stage>
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Staging(void);
   ~Staging(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

Staging* _pStaging(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
