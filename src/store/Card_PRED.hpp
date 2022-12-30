/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PRED : public Card
  {  
  public:
    Card_PRED(eCK eCard);
    Card_PRED(eCK eCard, Stz* szValue);
   ~Card_PRED(void) noexcept(false);

    bool bAllSubs_MaybeBoundsOf_index(Card_Index* p_index, bool bParamAllowedInBounds);
    Card_PRED* pContaining_MaybeBoundsOf_index(Card_Index* p_index, bool bParamAllowedInBounds);
    bool bDescend(std::function<bool(Card_PRED* )> bfLambdaFn);
    Card_CmpPred* pPredFindBound(Index* pIndex, ePB eBound);
    bool bPredHasBounds (Index* pIndex);
    bool bPredRecursive(std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply);
    char cTable_WhichTableFor_Pred(Card_Deck* p_deck);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
