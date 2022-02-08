/*============================================================================*/
/* Rules.hpp -- Optimiser Rules                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE { class Bench; }


namespace STORE
{
  class Rules : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Rules(void);
   ~Rules(void) noexcept(false);

  private:
  
  
    Footer* aFooter(void);
  };

  Rules* _pRules(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
