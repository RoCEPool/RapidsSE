/*============================================================================*/
/* Rule.hpp -- Abstract Rule                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eRK.h"

namespace STORE
{
  class Rule : public Object
  {
  protected:
    eRK  m_eRule;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Rule(eRK eRule);
   ~Rule(void) noexcept(false);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  protected:
    Footer* aFooter(void);
  };

  Rule* _pRule(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
