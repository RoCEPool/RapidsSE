/*============================================================================*/
/* Card_XCard.hpp -- Cross-Card                                               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_XCard : public Card_symbol<Card_XCard>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_XCard(void);
   ~Card_XCard(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_XCard* _pCard_XCard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
