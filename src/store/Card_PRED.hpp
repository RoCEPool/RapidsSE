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

    bool bContains_callToSubQuery(CardIter* pCI);
    bool bDescend(std::function<bool(Card_PRED* )> bfLambdaFn);
    Stz* szGetBoardNmInTerm(CardIter* pCI);
    Card_Term* pGetFirstTerm_tblColOdn(void);
    Card_Term* pGetTerm(i8 i_Term);
    Card_Term* pGetTerm_boardNm(CardIter* pCI);
    bool bIsIndexBounds_v3(Card_Index* p_index);
    bool bMaybeFilterPred(CardIter* pCI);
    bool bMaybeJoinPred(void);
    bool bMaybeSubQueryPred(void);
    Card_CmpPred* pPredFindBound(Index* pIndex, ePB eBound);
    bool bPredHasBounds (Index* pIndex);
    bool bPredRecursive(std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply);
    char cTable_WhichTableFor_Pred(CardIter* pCI);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
