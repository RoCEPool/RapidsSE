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
    Card_ELIST(eCK eCard);
   ~Card_ELIST(void) noexcept(false);

    void vBump_keyOdn_In_rpnCalc(CardIter* pCI);
    void vBump_stmOdn_In_rpnCalc(CardIter* pCI);
    void vBump_Rightwards(i8 nBumpBy);
    Card_Expr* pContains(Card_Expr* p_expr_arg);
    void vConvertAll_stmOdn_To_colOdn(CardIter* pCI);
    Card_Expr* pDecodeExpr(Symbol* pSymbol);
    i8 nExpr(void);
    Card_Expr* pFindExpr(Symbol* pSymbol);
    Card_Expr* pGetExpr(i8 i_Expr);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vRenumberAll_expr(void);
    i8 nRenumberAll_syntheticAs(void);
  };

  Card_ELIST* _pCard_ELIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
