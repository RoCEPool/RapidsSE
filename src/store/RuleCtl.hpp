/*============================================================================*/
/* RuleCtl.hpp -- Rule Control                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"

namespace STORE
{
  class RuleCtl : public Object
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RuleCtl(void);
   ~RuleCtl(void) noexcept(false);
  
  protected:
    Footer* aFooter(void);
  };

  RuleCtl* _pRuleCtl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
