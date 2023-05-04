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

    void vAppendRpnCalc(Card_RpnCalc* p_rpnCalc);
    void vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr=false);
    void vConvert_sqyOp_not_To_notSqyOp(void);
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    void vEnsureTypesFor_stmColOdn(CardIter* pCI);
    void vFactorAll_colOdn_IntoDistinct_expr(CardIter* pCI);
    Card_Term* pGetSingleTermIf_odn(void);
    Stz* szGetSingleTermIf_odn(void);
    Card_Term* pGetSingleTermIf_stmColOdn(void);
    Card_Term* pGetTerm(i8 i_Term);
    bool bHasWhen(void);
    bool bIsBraced(void);
    bool bIsSame(Card_RpnCalc* p_rpnCalc_arg);
    bool bIsSingle_colOdn(void);
    void vOntoStzList(StzList* pStzList);
    Card_BoolCalc* pToBoolCalc(CardIter* pCI);

    Stz* szP_Type(void);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
