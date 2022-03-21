/*============================================================================*/
/* RuleCtl.hpp -- Rule Control                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"
#include "eRN.h"

namespace STORE
{
  class RuleCtl : public Object
  {
  protected:
  
    b8 m_baFlag[RN_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RuleCtl(void);
   ~RuleCtl(void) noexcept(false);
  
    bool bFlag(i8 i_Flag);
    void vFlag(i8 i_Flag, bool bFlag);

  protected:
    Footer* aFooter(void);
  };

  RuleCtl* _pRuleCtl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
