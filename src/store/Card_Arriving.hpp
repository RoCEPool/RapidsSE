/*============================================================================*/
/* Card_Arriving.hpp -- Arriving columns                                      */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"
#include "ColsReq.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Arriving : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Arriving(void);
   ~Card_Arriving(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Arriving* _pCard_Arriving(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
