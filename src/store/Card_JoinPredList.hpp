/*============================================================================*/
/* Card_JoinPredList.hpp -- Join Predicate List                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinPredList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPredList(void);
   ~Card_JoinPredList(void) noexcept(false);

    static Card_JoinPredList* pExtract(Card_Deck* p_deck);

    void vConcat(Card_IndexList* p_indexList, Card_Filter* p_filter);

  };

  Card_JoinPredList* _pCard_JoinPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
