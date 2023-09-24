/*============================================================================*/
/* Card_Injected.hpp -- Injected columns                                      */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"
#include "ColsReq.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Injected : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Injected(void);
   ~Card_Injected(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Injected* _pCard_Injected(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
