/*============================================================================*/
/* Card_AbstractJoin.hpp -- Abstract Join                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_AbstractJoin : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_AbstractJoin(void);
    ~Card_AbstractJoin(void) noexcept(false);
  };

  Card_AbstractJoin* _pCard_AbstractJoin(void* aAddr);
  bool _bIsCard_AbstractJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
