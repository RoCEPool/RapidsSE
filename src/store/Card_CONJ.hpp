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

    bool bDescend(std::function<bool(Card_CONJ* )> bfLambdaFn);
    void vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr);
    Stz* szGetTerm_sqyNo(CardIter* pCI);
    Card_Term* pGetFirstTerm_tblCol(void);
    Card_Term* pGetTerm(i8 i_Term);
    Card_Term* pGetTerm_sqyNo(CardIter* pCI);
    bool bIsBoolFn(void);
    bool bIsCorrConj(void);
    bool bIsJoinConj(void);
    bool bIsRelnOp(void);
    bool bIsSqyOp(void);
    bool bLambdaWalk_conj(std::function<bool(Card_CONJ* p_pred, bool bUpwards)> bfLambdaFn);
    void vMorph_sqlOp_To_EQ(CardIter* pCI, Card_Term* p_term= nullptr);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    Symbol* pTable_WhichTableFor_Conj(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
