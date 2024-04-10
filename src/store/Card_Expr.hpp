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
    static void vAssemble_injCrlRsl(CardIter* pCI);

    static Card_Expr* pCraft_expr_Of_expr(Card_Expr* p_expr, i8 iExpCol);
    static Card_Expr* pCraft_expr_Of_symbol(Symbol* pSymbol);
    static Card_Expr* pCraft_expr_Of_term(Card_Term* p_term);
    static Card_Expr* pCraft_expr_keyCol_Of_term(Card_Term *p_term);
    static Card_Expr* pCraft_expr_of_nmdCol(Column *pColumn, i8 iExpCol,
                                            char cTblSyn, i8 iTblCol);

    void vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr);
    Card_Expr* pClone(void);
    bool bContains(CardIter* pCI, Symbol* pSymbol_arg);
    void vDeleteFacetIf(CardIter* pCI, eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsure_expr_Has_nmdCol(i8 iExpCol);

    Symbol* pFirst_colSym(CardIter* pCI);
    Symbol* pFirst_expCol(CardIter* pCI);
    Symbol* pFirst_keyCol(CardIter* pCI);
    Symbol* pFirst_tblCol(CardIter* pCI);

    Symbol*    pGet_Symbol(i8 i_Term);
    Card_Term* pGet_term(i8 i_Term);
    Card_Term* pGet_term0(void);

    bool bHasTerm(CardIter* pCI, Symbol* pSymbol);

    Card_Expr *pInduct(CardIter* pCI, i8 iExp);
    bool bIsCompound(void);
    bool bIsLiterals(CardIter* pCI);
    bool bIsScalarCol(void);
    bool bIsKeyCol(void);

    bool bMatches(Card_Expr* p_expr_arg);

    bool bNeedsBrace(CardIter* pCI);
    void vPrint(Ribbon* pRibbon, i8 iDepth);

    bool bSameAs(Card_Expr* pExpr_arg);

    void vUpdate_expCol(CardIter* pCI, Card_Emit* p_arriving);

    void vX_tblCol_To_expCol(CardIter* pCI);
    void vX_expCol_To_tblCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
