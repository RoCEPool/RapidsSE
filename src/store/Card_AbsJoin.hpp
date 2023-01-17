/*============================================================================*/
/* Card_AbsJoin.hpp -- Abstract Join                                          */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_AbsJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    static void vDeleteAllAbsJoin(CardIter* pCI);

    Card_AbsJoin(void);
    ~Card_AbsJoin(void) noexcept(false);
  };

  Card_AbsJoin* _pCard_AbsJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
