/*============================================================================*/
/* Card_CONDITION.hpp -- Abstract Condition                                        */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"
#include "Card_CONJUNCTION.hpp"

namespace STORE
{
  class Card_CONDITION : public Card_OP
  {
  public:
    Card_CONDITION(eCK eCard, i8 iFixedDepth);
   ~Card_CONDITION(void) noexcept(false);

   void vAddTransients(CardIter* pCI);
   void vX_tblCol_To_stmCol(CardIter* pCI);
   void vX_stmOdn_To_colOdn(CardIter* pCI);

    void vShortForm(String* p_string);

  };

  Card_CONDITION* _pCard_CONDITION(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
