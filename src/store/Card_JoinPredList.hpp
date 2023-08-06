/*============================================================================*/
/* Card_JoinPredList.hpp -- Join Predicate List                               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_JoinPredList : public Card_symbol<Card_JoinPredList>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPredList(void);
   ~Card_JoinPredList(void) noexcept(false);

    static Card_JoinPredList* pExtractList(Card_Deck* p_deck);

    static void vTablesWithNoIndexes(Card_Deck* p_deck, String* pString);

    void vBuildList(Card_Filter* pCard_cmpPred, Card_IndexList* p_indexList);

  };

  Card_JoinPredList* _pCard_JoinPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
