/*============================================================================*/
/* Card_Combined.hpp -- Combined                                              */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Combined : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    static void vDeleteAllCombined(CardIter* pCI);

    Card_Combined(void);
   ~Card_Combined(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Combined* _pCard_Combined(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
