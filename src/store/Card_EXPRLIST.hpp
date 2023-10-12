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
    void vBump_stmCol_In_rpnCalc(CardIter* pCI);
    void vBump_Rightwards(i8 nBumpBy);

    Card_Expr* pContains(Card_Expr* p_expr_arg);

    Card_Expr* pDecodeExpr(Symbol* pSymbol);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);

    i8 nExpr(void);

    Card_Expr* pFind_expr_By_as(Symbol* pSymbol);
    Card_Expr* pFind_expr_By_as_stripItem(Symbol* pSymbol);
    Card_Expr* pFind_expr_By_ordinal(Symbol* pSymbol);
    Card_Expr* pFind_expr_By_term(Symbol* pSymbol, eSF* aeFacet=nullptr);
    Card_Expr* pFind_expr_By_symbol(Symbol* pSymbol, Kid* aKid);

    Card_Expr* pGetExpr(i8 i_Expr);

    void vRenumberAll_expr(void);

    void vShortForm(String* pString);

    StreamDef* pToStreamDef(ColMap* pColMap,
                            Card_Key* p_key,
                            Card* p_OP);

    void vX_tblCol_To_stmCol(CardIter* pCI);
    void vX_stmCol_To_tblCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_EXPRLIST* _pCard_EXPRLIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
