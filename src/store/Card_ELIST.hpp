/*============================================================================*/
/* Card_ELIST.hpp -- Abstract Expression List                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_ELIST : public Card
  {  
  public:
    Card_ELIST(eCK eCard);
   ~Card_ELIST(void) noexcept(false);

    void vBump_keyOdn_In_rpnCalc(CardIter* pCI);
    void vBump_stmOdn_In_rpnCalc(CardIter* pCI);

    void vBump_Rightwards(i8 nBumpBy);

    Card_Expr* pContains(Card_Expr* p_expr_arg);

    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    Card_Expr* pDecodeExpr(Card_Term* p_term);
    i8 nExpr(void);
    Card_Expr* pFindExpr(Stz* szPrjCol);

    Card_Expr* pFind_expr_By_projectedCol(Stz* szCol);
    Card_Expr* pFind_expr_With_projectedCol(Card_Term* p_term_col);

    Card_Expr* pGetExpr(i8 i_Expr);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vRenumberAll_expr(void);
  };

  Card_ELIST* _pCard_ELIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
