/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_PRED : public Card_symbol<Card_PRED>
  {  
  public:
    Card_PRED(eCK eCard);
   ~Card_PRED(void) noexcept(false);

    static bool bIsPRED(Card* p_card);

    bool bContains_callToSubQuery(CardIter* pCI);
    bool bDescend(std::function<bool(Card_PRED* )> bfLambdaFn);
    void vEnsureTypesFor_stmOdn(CardIter* pCI);
    Stz* szGetBoardNmInTerm(CardIter* pCI);
    Card_Term* pGetFirstTerm_tblColOdn(void);
    Card_Term* pGetTerm(i8 i_Term);
    Card_Term* pGetTerm_boardNm(CardIter* pCI);
    bool bIsBoolFn(void);
    bool bIsCorrPred(void);
    bool bIsIndexBounds_v3(Card_Index* p_index);
    bool bIsJoinPred(void);
    bool bIsRelnOp(void);
    bool bIsSubQueryOp(void);
    bool bMaybeFilterPred(CardIter* pCI);
    bool bMaybeJoinPred(void);
    void vMorph_SqlOp_To_Eq(CardIter* pCI, Card_Term* p_term=nullptr);
    Card_CmpPred* pPredFindBound(Index* pIndex, ePB eBound);
    bool bPredHasBounds (Index* pIndex);
    bool bPredRecursive(std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply);
    Symbol* pTable_WhichTableFor_Pred(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
