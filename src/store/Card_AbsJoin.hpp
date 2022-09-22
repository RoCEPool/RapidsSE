/*============================================================================*/
/* Card_AbsJoin.hpp -- Abstract Join                                          */
/*============================================================================*/
#pragma once

#include "Card_MAIN.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_AbsJoin : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_AbsJoin(void);
    ~Card_AbsJoin(void) noexcept(false);


    bool bIsPredInScope(Card_PRED* pCard_PRED);

  };

  Card_AbsJoin* _pCard_AbsJoin(void* aAddr);
  bool _bIsCard_AbsJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
