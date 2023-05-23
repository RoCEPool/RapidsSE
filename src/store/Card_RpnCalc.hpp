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
    Card_RpnCalc(Symbol* pSymbol);
   ~Card_RpnCalc(void) noexcept(false);

    void vBump_stmOdn(i8 nBumpBy, bool bSkipFoldExpr=false);
    Card_RpnCalc* pClone(void);
    void vConcat(Card_RpnCalc* p_rpnCalc);
    void vConvert_sqyOp_not_To_notSqyOp(void);
    void vConvertAll_stmOdn_To_colOdn(CardIter* pCI);
    void vEnsureTypesFor_stmOdn(CardIter* pCI);
    void vFactorAll_valOdn_IntoDistinct_expr(CardIter* pCI);
    Card_Term* pGetTerm(i8 i_Term);
    bool bHasTerm(Symbol* pSymbol);
    bool bNeedsBrace(void);
    void vOntoStzList(StzList* pStzList);

    Card_RpnCalc *pReducedCopy(i8 i_Expr);

    bool bSameAs(Card_RpnCalc* p_rpnCalc_arg);
    Card_BoolCalc* pToBoolCalc(CardIter* pCI);

    Stz* szP_Type(void);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
