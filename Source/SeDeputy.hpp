/*============================================================================*/
/* SeDeputy.hpp -- the se> prompt deputy                                      */
/*============================================================================*/
#pragma once

#include "Prompt.h"

namespace Prompt
{
  class SeDeputy : public Object
  {
  private:
    SePrompt* m_pSePrompt;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SeDeputy(SePrompt* pSePrompt);
   ~SeDeputy(void) noexcept(false);
  
  private:

    Footer* aFooter(void);
  };

  SeDeputy* _pSeDeputy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
