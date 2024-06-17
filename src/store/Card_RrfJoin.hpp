/*============================================================================*/
/* Card_RrfJoin.hpp -- Row Ref Join                                           */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_RrfJoin : public Card_symbol<Card_RrfJoin>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RrfJoin(void);
   ~Card_RrfJoin(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_RrfJoin* _pCard_RrfJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
