/*============================================================================*/
/* Card_ELIST.hpp -- Abstract Expression List                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_StmColList.hpp"

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
  };

  Card_ELIST* _pCard_ELIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
