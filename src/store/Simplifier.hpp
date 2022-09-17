/*============================================================================*/
/* Simplifier.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissector.hpp"

namespace STORE
{
  class Simplifier : public Object
  {
  private:

    Card_SubSet* m_pCard_SubSet;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Simplifier(Card_SubSet* pCard_SubSet);
  
   ~Simplifier(void) noexcept(false);

    void vSimplify(void);
  private:

    Footer* aFooter(void);
  };

  Simplifier* _pSimplifier(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
