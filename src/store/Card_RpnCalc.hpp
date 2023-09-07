/*============================================================================*/
/* Card_RpnCalc.hpp -- RPN Calculation Sequence                               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_RpnCalc : public Card_symbol<Card_RpnCalc>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnCalc(void);
    Card_RpnCalc(Symbol* pSymbol);
   ~Card_RpnCalc(void) noexcept(false);

    void vBump_stmOdn(i8 nBumpBy, bool bSkipFoldExpr=false);
    eCT  eColumnType(void);
    Card_RpnCalc* pClone(void);
    void vConvert_stmOdn_To_colOdn(CardIter* pCI);
    void vCorrectStmOdn(Card_Projection* p_projection_prior);
    void vEnsureTypesFor_stmOdn(CardIter* pCI);
    void vFactorAll_valOdn_IntoDistinct_expr(CardIter* pCI);
    Symbol* pFirstKeyCol(void);
    Symbol* pFirstStmCol(void);
    Symbol* pFirstTblCol(void);
    Card_Term* pGetTerm(i8 i_Term);
    bool bHasTerm(Symbol* pSymbol);
    bool bIsAggCalc(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bNeedsBrace(void);
    void vOntoStzList(StzList* pStzList);
    Card_RpnCalc* pPropagate(Card_Expr* p_expr_encasing);
    Card_RpnCalc *pReducedCopy(i8 i_Expr,  bool bIs_SQCRLKEY);
    Card_RpnCalc *pReducedCopy_key(void);
    bool bSameAs(Card_RpnCalc* p_rpnCalc_arg);
    void vTransfer(Card_RpnCalc* p_rpnCalc, eSF eSymbol_limit);
    Card_BoolCalc* pTo_boolCalc(CardIter* pCI);
    Stz* szP_Type(void);
    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
