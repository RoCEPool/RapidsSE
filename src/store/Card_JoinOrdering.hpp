/*============================================================================*/
/* Card_JoinOrdering.hpp -- Join Order Sequence                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinOrdering : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinOrdering(void);
   ~Card_JoinOrdering(void) noexcept(false);

    static void vEstimateJoinedBuff(Card_Deck* pDeck, Card_TableList* p_tableList);
    static void vEstimateJoinOrder(Card_Deck* pDeck);
    static void vEstimateNetwork(Card_Deck* pDeck);

    void vConcat(Card_PtndJoin* p_PtndJoin);
  };

  Card_JoinOrdering* _pCard_JoinOrdering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
