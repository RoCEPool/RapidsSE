/*============================================================================*/
/* Card_RpnCalc.hpp -- RPN Calculation Sequence                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_RpnCalc : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnCalc(void);
   ~Card_RpnCalc(void) noexcept(false);

    static Card_RpnCalc* pDistinct_rpnCalc(Card_Term* p_term_colOdn);

    void vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr=false);
    void vConcat(Card_RpnCalc* p_rpnCalc);
    void vConvert_sqyOp_not_To_notSqyOp(void);
    void vConvert_stmOdn_terms_To_colOdn(CardIter* pCI);
    void vEnsureTypesFor_stmOdn(CardIter* pCI);
    void vFactorAll_colOdn_IntoDistinct_expr(CardIter* pCI);
    Card_Term* pGetSingleTermIf_odn(void);
    Stz* szGetSingleTermIf_odn(void);
    Card_Term* pGetSingleTermIf_stmColOdn(void);
    Card_Term* pGetTerm(i8 i_Term);
    bool bHasTerm(Stz* szSymbol);
    bool bNeedsBrace(void);
    bool bIsSingle_colOdn(void);
    void vOntoStzList(StzList* pStzList);
    bool bSameAs(Card_RpnCalc* p_rpnCalc_arg);
    Card_BoolCalc* pToBoolCalc(CardIter* pCI);

    Stz* szP_Type(void);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
