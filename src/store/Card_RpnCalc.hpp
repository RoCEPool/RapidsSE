/*============================================================================*/
/* Card_RpnCalc.hpp -- RPN Calculation Sequence                               */
/*============================================================================*/
#pragma once

#include "Card_TERMLIST.hpp"

namespace STORE
{
  // todo: Can remove this [296]
  class Card_RpnCalc : public Card_TERMLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnCalc(void);
    Card_RpnCalc(Symbol* pSymbol);
   ~Card_RpnCalc(void) noexcept(false);

    void vBump_expCol(i8 nBumpBy, bool bSkipFoldExpr=false);
    Card_RpnCalc* pClone(void);
    bool bContains(Symbol* pSymbol_arg);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    Symbol* pFirstColSym(void);
    Symbol* pFirstKeyCol(void);
    Symbol* pFirstNmdCol(void);
    Symbol* pFirstStmCol(void);
    Symbol* pFirstTblCol(void);
    Card_Term* pGetTerm(i8 i_Term);
    bool bHasTerm(Symbol* pSymbol);
    Card_RpnCalc* pInduct(Card_Expr* p_expr_arriving);
    bool bIsAggCalc(void);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bNeedsBrace(void);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
     bool bSameAs(Card_RpnCalc* p_rpnCalc_arg);
    void vShortForm(String* pString, Stz* szSep);
    void vTransfer(Card_RpnCalc* p_rpnCalc, eSF eSymbol_limit);
    Card_BoolPred* pTo_boolPred(void);
    void vX_expCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
