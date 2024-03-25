/*============================================================================*/
/* Card_CONSTRUCTION.hpp -- Construction                                      */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_CONSTRUCTION : public Card_OP
  {  
  public:
    Card_CONSTRUCTION(eCK eCard, i8 iFixedDepth);
   ~Card_CONSTRUCTION(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_CONSTRUCTION* _pCard_CONSTRUCTION(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
