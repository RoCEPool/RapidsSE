/*============================================================================*/
/* Card_CONJ.hpp -- Abstract Conjunction                                        */
/*============================================================================*/
#pragma once

#include "Card_CONJSUBROOT.hpp"

namespace STORE
{
  class Card_CONJ : public Card_CONJSUBROOT
  {  
  public:
    Card_CONJ(eCK eCard, i8 iFixedDepth);
   ~Card_CONJ(void) noexcept(false);

    static bool bIsConj(Card* p_card);

    bool bConjHasBounds (Index* pIndex);
    bool bConjRecursive(std::function<bool(Card_CONJ* p_pred, bool bUpwards)> bfApply);
    bool bContains_sqyCall(CardIter* pCI);
    bool bDescend(std::function<bool(Card_CONJ* )> bfLambdaFn);
    void vEnsureTypesFor_stmCol(CardIter* pCI);
    Stz* szGetBoardNmInTerm(CardIter* pCI);
    Card_Term* pGetFirstTerm_tblCol(void);
    Card_Term* pGetTerm(i8 i_Term);
    Card_Term* pGetTerm_boardNm(CardIter* pCI);
    bool bIsBoolFn(void);
    bool bIsCorrConj(void);
    bool bIsIndexBounds_v3(Card_Index* p_index);
    bool bIsJoinConj(void);
    bool bIsRelnOp(void);
    bool bIsSqyOp(void);
    void vMorph_sqlOp_To_EQ(CardIter* pCI, Card_Term* p_term= nullptr);

    Card_RlnConj* pPredFindBound(Index* pIndex, ePB eBound);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    Symbol* pTable_WhichTableFor_Conj(CardIter* pCI);
    void vShortForm(String* pString);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
