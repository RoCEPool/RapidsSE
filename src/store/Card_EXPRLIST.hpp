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

    void vBump_KeyCol_In_rpnCalc(CardIter* pCI);
    void vBump_expCol_In_rpnCalc(CardIter* pCI);
    void vBump_Rightwards(i8 nBumpBy);

    Card_Expr* pContains(Symbol* pSymbol_arg);
    Card_Expr* pContains(Card_Expr* p_expr_arg);

    Card_Term* pDecode_expr_term0(Symbol* pSymbol_expCol);

    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);

    void vEnsure_expr_Has_nmdCol(void);

    i8 nExpr(void);

    Card_Expr* pFind_expr_Matches(Symbol* pSymbol_arg);
    Card_Expr* pFind_expr_Matches(Symbol* pSymbol_arg, Kid* aKid);
    Card_Expr* pFind_expr_Matches(eSF eFacet_arg, Symbol* pSymbol_arg);
    Card_Expr* pFind_expr_Matches_ignoring(Symbol* pSymbol_arg, eSF eFacet_ignore);

    Card_Expr* pGet_expr(i8 i_Expr);

    void vRenumberAll_expr(void);

    bool bSame(Card_EXPRLIST* p_EXPRLIST_arg);

    bool bTestFind_expr_Matching(Card_Expr* p_expr_arg, Symbol* pSymbol_arg, eSF eFacet_ignore);
    StreamDef* pToStreamDef(ColMap* pColMap,
                            Card_Key* p_key,
                            Card* p_OP);

    void vUpdate_cast(Card_EXPRLIST* p_exprList_ref);

    void vX_tblCol_To_expCol(CardIter* pCI);
    void vX_expCol_To_tblCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_EXPRLIST* _pCard_EXPRLIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
