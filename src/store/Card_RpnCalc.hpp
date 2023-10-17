/*============================================================================*/
/* Card_RpnCalc.hpp -- RPN Calculation Sequence                               */
/*============================================================================*/
#pragma once

#include "Card_TERMLIST.hpp"

namespace STORE
{
  class Card_RpnCalc : public Card_TERMLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnCalc(void);
    Card_RpnCalc(Symbol* pSymbol);
   ~Card_RpnCalc(void) noexcept(false);

    void vBump_stmCol(i8 nBumpBy, bool bSkipFoldExpr=false);
    eCT  eColumnType(void);
    Card_RpnCalc* pClone(void);
    void vCorrectStmCol(Card_Emitted* p_emitted_prior);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsureTypesFor_stmCol(CardIter* pCI);
    void vFactorAll_ColSym_IntoDistinct_expr(CardIter* pCI);
    Symbol* pFirstColSym(void);
    Symbol* pFirstKeyCol(void);
    Symbol* pFirstStmCol(void);
    Symbol* pFirstTblCol(void);
    Card_Term* pGetTerm(i8 i_Term);
    bool bHasTerm(Symbol* pSymbol);
    bool bIsAggCalc(void);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bNeedsBrace(void);
    Card_RpnCalc* pPropagate(Card_Expr* p_expr_encasing);
    Card_RpnCalc *pReducedCopy(i8 i_Expr,  bool bIs_SQCRLKEY);
    Card_RpnCalc *pReducedCopy_key(void);
    bool bSameAs(Card_RpnCalc* p_rpnCalc_arg);
    void vShortForm(String* pString, Stz* szSep);
    void vTransfer(Card_RpnCalc* p_rpnCalc, eSF eSymbol_limit);
    Card_BoolPred* pTo_boolCalc(CardIter* pCI);
    eCT  eColType(void);
     void vX_stmCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_stmCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
