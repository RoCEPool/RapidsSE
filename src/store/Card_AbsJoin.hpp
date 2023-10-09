/*============================================================================*/
/* Card_AbsJoin.hpp -- Abstract Join                                          */
/*============================================================================*/
#pragma once

#include "Card_CONSTRUCTION.hpp"

namespace STORE
{
  class Card_AbsJoin : public Card_CONSTRUCTION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_AbsJoin(void);
    ~Card_AbsJoin(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_AbsJoin* _pCard_AbsJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
