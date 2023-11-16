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
   ~Card_Expr(void) noexcept(false);

    //-----------------------------------------------------------
    static Card_Expr* pGby_expr_From_expr_corr(Card_Expr* pExpr_corr);
    static Card_Expr* pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs=nullptr);
    static Card_Expr* pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col);

    static Card_Expr* pCast_expr_Column(Column *pColumn, i8 iExpCol,
                                        char cTblSyn, i8 iColOdn);

    void vBump_expCol(i8 nBumpBy, bool bSkipFoldExpr=false);
    Card_Expr* pClone(void);
    bool bContains(Symbol* pSymbol_arg);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsureTypeId_expCol(CardIter* pCI);
    Symbol* pFirst_expCol(void);
    Symbol* pFirst_keyCol(void);
    Symbol* pFirst_tblCol(void);
    Symbol* pGet_Symbol(i8 i_Term);
    Card_Term* pGet_term(i8 i_Term);
    Card_Term* pGet_term0(void);
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
    Card_Expr *pReducedCopy(i8 i_Expr);

    void vRenumberForm_keyCol(i8 i_Expr);
    void vRenumberForm_expCol(i8 i_Expr);

    bool bSameAs(Card_Expr* pExpr_arg);

    void vShortForm(String* pString, Stz* szSep);

    void vX_tblCol_To_expCol(CardIter* pCI);
    void vX_expCol_To_tblCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
