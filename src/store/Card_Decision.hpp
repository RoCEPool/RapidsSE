/*============================================================================*/
/* Card_Decision.hpp -- Decision                                              */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Decision : public Card_symbol<Card_Decision>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Decision(void);
   ~Card_Decision(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Decision* _pCard_Decision(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
