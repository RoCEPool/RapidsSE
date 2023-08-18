/*============================================================================*/
/* Card_On.hpp -- On Predicate                                                */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_On : public Card_symbol<Card_On>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_On(void);
   ~Card_On(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_On* _pCard_On(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
