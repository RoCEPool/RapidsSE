/*============================================================================*/
/* Card_CONJUNCTION.hpp -- Abstract Conjunction                                        */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_CONJUNCTION : public Card_symbol<Card_CONJUNCTION>
  {  
  public:
    Card_CONJUNCTION(eCK eCard, i8 iFixedDepth);
   ~Card_CONJUNCTION(void) noexcept(false);

    static bool bIsPRED(Card* p_card);

    bool bContains_callToSubQuery(CardIter* pCI);
    bool bDescend(std::function<bool(Card_CONJUNCTION* )> bfLambdaFn);
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
    Card_CmpConj* pPredFindBound(Index* pIndex, ePB eBound);
    bool bPredHasBounds (Index* pIndex);
    bool bPredRecursive(std::function<bool(Card_CONJUNCTION* p_pred, bool bUpwards)> bfApply);
    Symbol* pTable_WhichTableFor_Pred(CardIter* pCI);
    void vShortForm(String* pString);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
