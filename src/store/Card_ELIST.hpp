/*============================================================================*/
/* Card_ELIST.hpp -- Abstract Expression List                                 */
/*============================================================================*/
#pragma once

#include "Card_LIST.hpp"

namespace STORE
{
  class Card_ELIST : public Card_LIST
  {  
  public:
    Card_ELIST(eCK eCard, i8 iFixedDepth=C_UNKNOWN);
   ~Card_ELIST(void) noexcept(false);

    void vBump_keyOdn_In_rpnCalc(CardIter* pCI);
    void vBump_stmOdn_In_rpnCalc(CardIter* pCI);
    void vBump_Rightwards(i8 nBumpBy);
    Card_Expr* pContains(Card_Expr* p_expr_arg);
    void vConvertAll_stmOdn_To_colOdn(CardIter* pCI);
    Card_Expr* pDecodeExpr(Symbol* pSymbol);
    i8 nExpr(void);
    Card_Expr* pFindExpr(Symbol* pSymbol);
    Card_Expr* pFind_expr_By_as(Symbol* pSymbol);
    Card_Expr* pFind_expr_Of_term(Symbol* pSymbol);

    Card_Expr* pGetExpr(i8 i_Expr);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vRenumberAll_expr(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_ELIST* _pCard_ELIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
