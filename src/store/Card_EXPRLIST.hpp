/*============================================================================*/
/* Card_EXPRLIST.hpp -- Abstract Expression List                              */
/*============================================================================*/
#pragma once

#include "Card_LIST.hpp"

#include "ColMap.hpp"

namespace STORE
{
  class Kid;

  class Card_EXPRLIST : public Card_LIST
  {
  public:
    Card_EXPRLIST(eCK eCard, i8 iFixedDepth);
   ~Card_EXPRLIST(void) noexcept(false);

    void vApplyCast(CardIter* pCI, Card_EXPRLIST* p_Dic_expr);
    void vApplyFacets(CardIter* pCI, Card_EXPRLIST* p_key);

    void vBump_expCol(CardIter* pCI);
    void vBump_keyCol(CardIter* pCI);

    void vDeleteIf_facet(CardIter* pCI, eSF eFacet, eSF eAlsoFacet, char cTblSyn);

    void vEnsureAll_expr_Have_nmdCol(void);
    bool bEqualTo(CardIter *pCI, Card_EXPRLIST* p_EXPRLIST);
    i8 nExpr(void);

    Card_Expr* pFind_expr(Card_Expr* p_expr_arg);
    Card_Expr* pFind_expr(Symbol* pSymbol_arg);
    Card_Expr* pFind_expr(Symbol* pSymbol_arg, Kid* aKid);
    Card_Expr* pFind_expr(eSF eFacet_arg, Symbol* pSymbol_arg);
    Card_Expr* pFind_expr(CardIter *pCI, Symbol* pSymbol);
    Card_Expr* pFind_expr_ignoring(Symbol* pSymbol_arg, eSF eFacet_ignore);
    Card_Expr* pFind_term(CardIter* pCI, Symbol* pSymbol_arg);
    Card_Term* pFind_term0_Of_expr(Symbol* pSymbol_expCol);
    Card_Expr* pFind_expr_key_Matches(CardIter *pCI, Symbol *pSymbol_arg);
    Card_Expr* pFind_expr_key_Matches(CardIter *pCI, eSF eFacet_arg, Symbol* pSymbol_arg);


    Card_Expr* pGet_expr(i8 i_Expr);
    Card_Expr* pGet_expr(Symbol* pSymbol_expCol);

    bool bIsCompound(void);
    bool bIsLiterals(CardIter* pCI);

    void vRenumberAll_expr(eSF eFacet=SF_EXP_COL);

    StreamDef* pTo_StreamDef(ColMap* pColMap,
                             Card_Key* p_key,
                             Card* p_OP);

    void vX_tblCol_To_expCol(CardIter* pCI);
    void vX_expCol_To_tblCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_EXPRLIST* _pCard_EXPRLIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
