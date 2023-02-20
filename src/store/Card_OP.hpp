/*============================================================================*/
/* Card_OP.hpp -- Abstract Op                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_OP : public Card
  {  
  public:
    Card_OP(eCK eCard);
   ~Card_OP(void) noexcept(false);

   void vAddLevel(void);
   void vAdd_Pred_Under_filter(Card_PRED *p_PRED);
   void vConvert_rpnCalc_To_boolCalc(CardIter* pCI);
   void vConvert_stmColOdn_To_tblColOdn_original(CardIter* pCI);
   void vConvert_stmColOdn_To_tblColOdn_recursive(CardIter* pCI);
   void vConvert_tblColOdn_To_stmColOdn(CardIter* pCI);
  };

  Card_OP* _pCard_OP(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
