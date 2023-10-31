/*============================================================================*/
/* Card_Expr.hpp -- Typed Expression                                          */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Expr : public Card_symbol<Card_Expr>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Expr(void);
    Card_Expr(Symbol* pSymbol);
    Card_Expr(Card_As*   p_as_arg,
              Card_Term* p_term_0_arg);
   ~Card_Expr(void) noexcept(false);

    //-----------------------------------------------------------
    static Card_Expr* pGby_expr_From_expr_corr(Card_Expr* pExpr_corr);
    static Card_Expr* pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs=nullptr);
    static Card_Expr* pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col);
    static Card_Expr* pProj_Simple_expr_Of_key(Card_Expr* p_expr_key);

    void vBump_stmCol(i8 nBumpBy, bool bSkipFoldExpr=false);
    eCT  eColumnType(void);
    Card_Expr* pClone(void);
    bool bContains(Symbol* pSymbol_arg);
    Card_Expr* pCraft_expr_early(CardIter* pCI, i8 i_Expr=0);
    void vDeconstruct(Card_As*&   rp_as,
                      Card_Term*& rp_term_0);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsureTypes_For_stmCol(CardIter* pCI);
    void vFactorAll_ColSym_IntoDistinct_expr(CardIter* pCI);
    Symbol* pFirstColSym(void);
    Symbol* pFirstKeyCol(void);
    Symbol* pFirstStmCol(void);
    Symbol* pFirstTblCol(void);
    Symbol* pGet_Symbol(i8 i_Term);
    Card_Term* pGet_term(i8 i_Term);
    Card_Term* pGet_term0(void);
    bool bIsAggCalc(void);
    bool bIsDescending(void);
    bool bMatches_term0(Symbol* pSymbol_arg, eSF* aeFacet_arg= nullptr);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bIsExpressionOfKey(void);
    bool bHasTerm(Symbol* pSymbol);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    Card_Expr* pPropagate(void);
    void vReconstruct(Card_As*    p_as,
                      Card_Term*  p_term_0);
    Card_Expr *pReducedCopy(i8 i_Expr);

    void vRenumberForm_KeyCol(i8 i_Expr);
    void vRenumberForm_stmCol(i8 i_Expr);

    bool bSameAs(Card_Expr* pExpr_arg);

    void vShortForm(String* pString, Stz* szSep);

    eCT  eColType(void);

    void vX_tblCol_To_stmCol(CardIter* pCI);
    void vX_stmCol_To_tblCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
