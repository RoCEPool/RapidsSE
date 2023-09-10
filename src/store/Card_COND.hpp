/*============================================================================*/
/* Card_COND.hpp -- Abstract Condition                                        */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"
#include "Card_CONJUNCTION.hpp"

namespace STORE
{
  class Card_COND : public Card_OP
  {
  public:
    Card_COND(eCK eCard, i8 iFixedDepth);
   ~Card_COND(void) noexcept(false);

   void vAddTransients(CardIter* pCI);
   void vConvert_colOdn_To_stmOdn(CardIter* pCI);
   void vConvert_stmOdn_To_colOdn(CardIter* pCI);


  };

  Card_COND* _pCard_COND(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
