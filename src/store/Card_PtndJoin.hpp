/*============================================================================*/
/* Card_PtndJoin.hpp -- Partitioned Join                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PtndJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PtndJoin(void);
   ~Card_PtndJoin(void) noexcept(false);

    static void vEstimatePtndJoins(Card_Deck* pDeck, Card_TableList* p_tableList);

    void vEstimateJoinBuff(Card_TableList* p_tableList);
    bool bMayFollowAfter(Card_SubSet* pPriorPtnJoins_sofar);
  };

  Card_PtndJoin* _pCard_PtndJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
