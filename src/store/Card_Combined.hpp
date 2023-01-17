/*============================================================================*/
/* Card_Combined.hpp -- Combined                                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Combined : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    static void vDeleteAllCombined(CardIter* pCI);

    Card_Combined(void);
   ~Card_Combined(void) noexcept(false);
  };

  Card_Combined* _pCard_Combined(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
