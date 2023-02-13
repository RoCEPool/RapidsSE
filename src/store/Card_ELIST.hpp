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
    i8 iRpnCalc_Has_stmOrdOdn(Card_OrdElmOdn* p_stmOrdOdn);


    void vBump_Rightwards(i8 nBumpBy);
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    i8 nExpr(void);
    Card_Expr* pGetExpr(i8 i_Expr);
    Card_Expr* pGetExpr(Card_Term* p_term);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vRenumberAllExpr(void);

  };

  Card_ELIST* _pCard_ELIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
