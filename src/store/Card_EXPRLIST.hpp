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

    void vBump_keyOdn_In_rpnCalc(CardIter* pCI);
    void vBump_stmOdn_In_rpnCalc(CardIter* pCI);
    void vBump_Rightwards(i8 nBumpBy);

    Card_Expr* pContains(Card_Expr* p_expr_arg);
    void vConvert_colOdn_To_stmOdn(CardIter* pCI);
    void vConvert_stmOdn_To_colOdn(CardIter* pCI);

    Card_Expr* pDecodeExpr(Symbol* pSymbol);

    i8 nExpr(void);

    Card_Expr* pFind_expr_By_as(Symbol* pSymbol);
    Card_Expr* pFind_expr_By_as_stripItem(Symbol* pSymbol);
    Card_Expr* pFind_expr_By_as_stripped(Symbol* pSymbol, Stz* szTblCol, Kid* aKid);
    Card_Expr* pFind_expr_By_ordinal(Symbol* pSymbol);
    Card_Expr* pFind_expr_By_symbol(Symbol* pSymbol);

    Card_Expr* pGetExpr(i8 i_Expr);

    void vRenumberAll_expr(void);

    void vShortForm(String* pString);

    StreamDef* pToStreamDef(ColMap* pColMap,
                            Card_Key* p_key,
                            Card* p_OP);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_EXPRLIST* _pCard_EXPRLIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
