/*============================================================================*/
/* Card_AbsJoin.hpp -- Abstract Join                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_AbsJoin : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_AbsJoin(void);
    ~Card_AbsJoin(void) noexcept(false);
  };

  Card_AbsJoin* _pCard_AbsJoin(void* aAddr);
  bool _bIsCard_AbsJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
