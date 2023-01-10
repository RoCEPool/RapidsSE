/*============================================================================*/
/* Card_Deck.hpp -- Deck of Cards                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Deck : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Deck(void);
   ~Card_Deck(void) noexcept(false);

    static void vApply(Card_JoinOrder* p_joinOrder);
    static bool bOrder_Joins(CardIter* pCI);
  };

  Card_Deck* _pCard_Deck(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
