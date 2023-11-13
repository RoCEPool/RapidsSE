/*============================================================================*/
/* Card_CONJSUBROOT.hpp -- Abstract Conjunction Sub-Root                      */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_CONJSUBROOT : public Card_symbol<Card_CONJSUBROOT>
  {  
  public:
    Card_CONJSUBROOT(eCK eCard, i8 iFixedDepth);
   ~Card_CONJSUBROOT(void) noexcept(false);

    static void vOntoRpnExpr(String* psRpnExpr, Card_CONJSUBROOT* p_conj);

    Card_AndConj* pGet_andConj(i8 i_Child);
    Card_NotConj* pGet_notConj(i8 i_Child);
    Card_OrConj*  pGet_orConj (i8 i_Child);
    Card_RlnConj* pGet_rlnConj(i8 i_Child);

    void vOntoRpnExpr(String* psRpnExpr);

    Card_AndConj* pPop_andConj(void);
    Card_NotConj* pPop_notConj(void);
    Card_OrConj*  pPop_orConj (void);
    Card_RlnConj* pPop_rlnConj(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
